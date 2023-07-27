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
pub const DOCKER_MANIFEST: MimeView =
    MimeView::new_const("application/vnd.docker.distribution.manifest.v2+json");

pub const DOCKER_ROOTFS_DIFF_TAR_GZ: MimeView =
    MimeView::new_const("application/vnd.docker.image.rootfs.diff.tar.gzip");

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

impl Mime {
    pub fn view(&self) -> MimeView {
        MimeView { v: self.v.as_ref() }
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

impl<'a> serde::Deserialize<'a> for Mime {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: serde::Deserializer<'a>,
    {
        Ok(Mime::new(String::deserialize(deserializer)?))
    }
}

impl<T: MimeData> serde::Serialize for BasicMime<T> {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer,
    {
        serializer.serialize_str(&self.v.as_ref())
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
