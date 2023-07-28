use std::{convert::AsRef, fmt::Debug};


pub trait DigestData: AsRef<str> + Debug + Into<String> {}
impl<T: AsRef<str> + Debug + Into<String>> DigestData for T {}


#[derive(Eq)]
pub struct BasicDigest<T: DigestData>(T);

pub type Digest = BasicDigest<String>;
pub type DigestView = BasicDigest<&str>;


#[derive(Eq)]
pub struct BasicDigest<T: DigestData> {
    v: T,
}

pub type Digest = BasicDigest<String>;
pub type DigestView<'a> = BasicDigest<&'a str>;

pub enum DigestErr {
    Placeholder,
}

impl<T> BasicDigest<T>
where
    T: DigestData,
{
    /// Crate a new digest
    pub fn new(v: impl Into<T>) -> BasicDigest<T> {
        BasicDigest { v: v.into() }
    }

    /// Initialization working in compile time
    pub const fn new_const(v: T) -> BasicDigest<T> {
        BasicDigest { v }
    }

    /// Access the algorithm field of the digest
    pub fn algo(&self) -> &str {
        let rf = self.v.as_ref();
        match rf.find(':') {
            Some(i) => &rf[0..i],
            None => "",
        }
    }

    /// Access the value part of the digest
    pub fn value(&self) -> &str {
        let rf = self.v.as_ref();
        match rf.find(':') {
            Some(i) => &rf[i + 1..],
            None => &rf,
        }
    }

    pub fn str(&self) -> &str {
        self.v.as_ref()
    }
}

impl<'a, 'b: 'a> Digest {
    pub fn view(&'b self) -> DigestView<'a> {
        DigestView {
            v: &self.v.as_ref(),
        }
    }
}

impl<T> AsRef<str> for BasicDigest<T>
where
    T: DigestData,
{
    fn as_ref(&self) -> &str {
        self.v.as_ref()
    }
}

impl<T> Into<String> for BasicDigest<T>
where
    T: DigestData,
{
    fn into(self) -> String {
        self.v.into()
    }
}

impl<T, U> PartialEq<U> for BasicDigest<T>
where
    T: DigestData,
    U: AsRef<str>,
{
    fn eq(&self, other: &U) -> bool {
        self.as_ref() == other.as_ref()
    }
}

impl<T> Debug for BasicDigest<T>
where
    T: DigestData,
{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.v.fmt(f)
    }
}

impl<'a> serde::Deserialize<'a> for Digest {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: serde::Deserializer<'a>,
    {
        Ok(Digest::new(String::deserialize(deserializer)?))
    }
}

impl<T: DigestData> serde::Serialize for BasicDigest<T> {
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
    fn new_digest() {
        let dig = Digest::new("foobar");
        assert_eq!(dig.as_ref(), "foobar");
        assert_eq!(dig, "foobar");
    }

    #[test]
    fn new_digest_view() {
        let dig = DigestView::new("foobar");
        assert_eq!(dig.as_ref(), "foobar");
    }

    #[test]
    fn digest_from_view() {
        let view = DigestView::new("foo:bar");
        let dig = Digest::new(view);
        assert_eq!(dig, "foo:bar");
    }

    #[test]
    fn algo_and_value() {
        let dig = DigestView::new("foo:bar");
        assert_eq!(dig.algo(), "foo");
        assert_eq!(dig.value(), "bar");
        let dig = DigestView::new("asdfasdf");
        assert_eq!(dig.algo(), "");
        assert_eq!(dig.value(), "asdfasdf");
    }
}
