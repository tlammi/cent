use crate::DigestView;
use rusqlite::{Connection, Savepoint};

pub struct Storage {
    conn: Connection,
}

pub struct StorageLock<'a> {
    sp: Savepoint<'a>,
}

fn db_path(path: &str) -> String {
    std::path::Path::new(path)
        .join("storage.db")
        .into_os_string()
        .into_string()
        .unwrap()
}

impl Storage {
    pub fn new(path: &str) -> Option<Storage> {
        let mut conn;
        if path == ":memory:" {
            conn = Connection::open_in_memory().ok()?;
        } else {
            conn = Connection::open(db_path(path).as_str()).ok()?;
        }
        Self::init_tables(&mut conn);
        Some(Storage { conn })
    }

    pub fn lock(&mut self) -> StorageLock {
        StorageLock {
            sp: self.conn.savepoint().unwrap(),
        }
    }

    fn init_tables(c: &mut Connection) -> Option<()> {
        let sp = c.savepoint().ok()?;
        let _ = (*sp).execute(
            "CREATE TABLE IF NOT EXISTS layers (digest TEXT NOT NULL PRIMARY KEY, data BLOB)",
            (),
        );
        sp.commit().ok()?;
        Some(())
    }
}

impl StorageLock<'_> {
    pub fn store_layer(&mut self, d: DigestView, data: &[u8]) -> Option<()> {
        let mut stmt = (*self.sp)
            .prepare("INSERT INTO layers VALUES (?, ?)")
            .ok()?;
        stmt.execute((d.str(), data)).ok()?;
        Some(())
    }

    pub fn layer(&mut self, d: DigestView) -> Option<Vec<u8>> {
        let mut stmt = (*self.sp)
            .prepare("SELECT data from layers WHERE (digest=?)")
            .ok()?;
        let layer = stmt.query_row((d.str(),), |row| row.get(0));
        layer.ok()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn open_storage() {
        let mut _s = Storage::new(":memory:").unwrap();
    }

    #[test]
    fn store_layer() {
        let mut s = Storage::new(":memory:").unwrap();
        let mut lk = s.lock();
        lk.store_layer(DigestView::new("foo:bar"), &[1, 2, 3, 4])
            .unwrap();
    }

    #[test]
    fn layer_roundtrip() {
        let mut s = Storage::new(":memory:").unwrap();
        let mut lk = s.lock();
        lk.store_layer(DigestView::new("foo:bar"), &[1, 2, 3, 4])
            .unwrap();
        let data = lk.layer(DigestView::new("foo:bar")).unwrap();
        assert!(data == [1, 2, 3, 4]);
    }
}
