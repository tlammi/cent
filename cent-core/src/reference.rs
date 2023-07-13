
use std::fmt::Debug;

pub trait ReferenceData: AsRef<str> + Debug {}
impl<T: AsRef<str> + Debug> ReferenceData for T {}

pub struct BasicReference<T>{
    s: T,
}

pub type Reference = BasicReference<String>;
pub type ReferenceView<'a> = BasicReference<&'a str>;


impl<T: ReferenceData> BasicReference<T>{

    pub fn new(s: impl Into<T>) -> BasicReference<T>{
        BasicReference{s: s.into()}
    }

    pub fn tag(&self) -> Option<&str> {
        let s = self.s.as_ref();
        match (s.find(':'), s.find('@')) {
            (_, Some(_)) => None,
            (None, _) => Some("latest"),
            (Some(i), _) => Some(&s[i+1..]),
        }
    }

    pub fn digest(&self) -> Option<&str> {
        let s = self.s.as_ref();
        s.find('@').map(|i| &s[i+1..])
    }

    pub fn registry(&self) -> Option<&str> {
        let s = self.s.as_ref();
        s.find('/').map(|i| &s[0..i])
    }

    pub fn name(&self) -> &str {
        let mut s = self.s.as_ref();
        self.ref_sep().map(|i| s = &s[..i]);
        s.find('/').map(|i| s = &s[i+1..]);
        s
    }

    pub fn repostitory(&self) -> &str {
        let s = self.s.as_ref();
        self.ref_sep().map_or(&s, |i| &s[..i])
    }

    pub fn is_empty(&self) -> bool {
        self.s.as_ref().is_empty()
    }

    fn ref_sep(&self) -> Option<usize> {
        let s = self.s.as_ref();
        match (s.find(':'), s.find('@')) {
            (_, Some(i)) => Some(i),
            (Some(i), None) => Some(i),
            (None, None) => None,
        }
    }
}

impl<T: ReferenceData, U: AsRef<str>> PartialEq<U> for BasicReference<T> {
    fn eq(&self, other: &U) -> bool {
        let tmp = ReferenceView::new(other.as_ref());
        self.repostitory() == tmp.repostitory() && self.tag() == tmp.tag() && self.digest() == tmp.digest()
    }
}

impl<T: ReferenceData> Debug for BasicReference<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.s.fmt(f)
    }
}

#[cfg(test)]
mod tests {

    use crate::*;

    #[test]
    fn new_ref() {
        let r = Reference::new("foobar");
        assert_eq!(r.is_empty(), false);
    }

    #[test]
    fn tag() {
        let mut r = Reference::new("foo");
        assert_eq!(r.tag().unwrap(), "latest");
        r = Reference::new("foo:latest");
        assert_eq!(r.tag().unwrap(), "latest");
        r = Reference::new("foo:bar");
        assert_eq!(r.tag().unwrap(), "bar");
        r = Reference::new("foo@sha256:deadbeef");
        assert_eq!(r.tag().is_none(), true);
    }

    #[test]
    fn repository(){
        let mut r = Reference::new("foo");
        assert_eq!(r.repostitory(), "foo");
        r = Reference::new("foo:bar");
        assert_eq!(r.repostitory(), "foo");
        r = Reference::new("foo@sha256:baz");
        assert_eq!(r.repostitory(), "foo");
    }
}
