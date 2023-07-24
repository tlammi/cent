use std::fmt::Debug;

pub trait MimeData: AsRef<str> + Debug + Into<String> {}
impl<T: AsRef<str> + Debug + Into<String>> MimeData for T {}

pub struct BasicMime<T> {
    v: T,
}

pub type Mime = BasicMime<String>;
pub type MimeView<'a> = BasicMime<&'a str>;

pub const DOCKER_MANIFEST_LIST: MimeView =
    MimeView::new_const("application/vnd.docker.distribution.manifest.list.v2+json");

impl<T> BasicMime<T>
where
    T: MimeData,
{
    pub fn new(v: impl Into<T>) -> BasicMime<T> {
        BasicMime { v: v.into() }
    }

    pub const fn new_const(v: T) -> BasicMime<T> {
        BasicMime { v }
    }

    pub fn toptype(&self) -> &str {
        let v = self.v.as_ref();
        match v.find('/') {
            Some(i) => &v[0..i],
            None => &v,
        }
    }

    pub fn subtype(&self) -> &str {
        let v = self.v.as_ref();
        match v.find('/') {
            Some(i) => &v[i + 1..],
            None => "",
        }
    }

    pub fn to_string(&self) -> String {
        self.v.as_ref().into()
    }
}

impl<T> Debug for BasicMime<T>
where
    T: MimeData,
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.v.fmt(f)
    }
}

impl<T> AsRef<str> for BasicMime<T>
where
    T: MimeData,
{
    fn as_ref(&self) -> &str {
        self.v.as_ref()
    }
}

impl<T, U> PartialEq<U> for BasicMime<T>
where
    T: MimeData,
    U: AsRef<str>,
{
    fn eq(&self, other: &U) -> bool {
        self.as_ref() == other.as_ref()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn new_mime() {
        let m = Mime::new("foo/bar");
        assert_eq!(m.toptype(), "foo");
        assert_eq!(m.subtype(), "bar");
    }

    #[test]
    fn new_mime_view() {
        let m = MimeView::new("foo/bar");
        assert_eq!(m.toptype(), "foo");
        assert_eq!(m.subtype(), "bar");
    }
}
