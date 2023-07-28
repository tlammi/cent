use crate::DigestView;
use std::fmt::Debug;

pub trait ReferenceData: AsRef<str> + Debug + Clone {}
impl<T: AsRef<str> + Debug + Clone> ReferenceData for T {}

pub enum Suffix<'a> {
    Null,
    Tag(&'a str),
    Digest(DigestView<'a>),
}

pub struct BasicReference<T>(T);

pub type Reference = BasicReference<String>;
pub type ReferenceView<'a> = BasicReference<&'a str>;

impl<T: ReferenceData> BasicReference<T> {
    pub fn new(s: impl Into<T>) -> BasicReference<T> {
        BasicReference(s.into())
    }

    pub fn suffix(&self) -> Suffix {
        let maybe_tag = self.tag();
        if maybe_tag.is_some() {
            return Suffix::Tag(maybe_tag.unwrap());
        }
        let maybe_dig = self.digest();
        if maybe_dig.is_some() {
            return Suffix::Digest(maybe_dig.unwrap());
        }
        Suffix::Null
    }

    pub fn tag(&self) -> Option<&str> {
        let s = self.0.as_ref();
        match (s.find(':'), s.find('@')) {
            (_, Some(_)) => None,
            (None, _) => Some("latest"),
            (Some(i), _) => Some(&s[i + 1..]),
        }
    }

    pub fn digest(&self) -> Option<DigestView> {
        let s = self.0.as_ref();
        let v = s.find('@').map(|i| DigestView::new(&s[i + 1..]));
        return v;
    }

    pub fn digest_or_tag(&self) -> &str {
        let s = self.0.as_ref();
        let dig = s.find('@').map(|i| &s[i + 1..]);
        if dig.is_some() {
            return dig.unwrap();
        }
        self.tag().unwrap_or("latest")
    }

    pub fn registry(&self) -> Option<&str> {
        let s = self.0.as_ref();
        s.find('/').map(|i| &s[0..i])
    }

    pub fn name(&self) -> &str {
        let mut s = self.0.as_ref();
        self.ref_sep().map(|i| s = &s[..i]);
        s.find('/').map(|i| s = &s[i + 1..]);
        s
    }

    pub fn repostitory(&self) -> &str {
        let s = self.0.as_ref();
        self.ref_sep().map_or(&s, |i| &s[..i])
    }

    pub fn is_empty(&self) -> bool {
        self.0.as_ref().is_empty()
    }

    fn ref_sep(&self) -> Option<usize> {
        let s = self.0.as_ref();
        match (s.find(':'), s.find('@')) {
            (_, Some(i)) => Some(i),
            (Some(i), None) => Some(i),
            (None, None) => None,
        }
    }

    /// Get HTTP URL of the registry this reference points to
    pub fn registry_root_url(&self) -> Option<String> {
        let reg = self.registry()?;
        if reg == "docker.io" {
            return Some("https://registry-1.".to_owned() + reg + "/v2/");
        }
        return Some("https://".to_owned() + reg + "/v2/");
    }

    /// Resolve URL where to find manifests or manifest lists for this reference
    pub fn manifest_url(&self) -> String {
        let mut url = self.registry_root_url().unwrap();
        let nm = self.name();
        let ref_ = self.digest_or_tag();
        url = url + nm + "/manifests/" + ref_;
        return url;
    }

    pub fn blob_url(&self) -> String {
        let url = self.registry_root_url().unwrap();
        let nm = self.name();
        let ref_ = self.digest_or_tag();
        url + nm + "/blobs/" + ref_
    }
}

impl Reference {
    pub fn set_suffix(&mut self, s: Suffix) {
        use Suffix::*;
        let mut slice: &str = self.0.as_ref();
        match self.ref_sep() {
            Some(i) => slice = &slice[..i],
            None => (),
        };
        match s {
            Null => (),
            Tag(st) => self.0 = slice.to_string() + ":" + st,
            Digest(st) => self.0 = slice.to_string() + "@" + st.str(),
        }
    }

    pub fn view(&self) -> ReferenceView {
        ReferenceView::new(self.0.as_str())
    }
}

impl<T: ReferenceData, U: AsRef<str>> PartialEq<U> for BasicReference<T> {
    fn eq(&self, other: &U) -> bool {
        let tmp = ReferenceView::new(other.as_ref());
        self.repostitory() == tmp.repostitory()
            && self.tag() == tmp.tag()
            && self.digest() == tmp.digest()
    }
}

impl<T: ReferenceData> Debug for BasicReference<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.0.fmt(f)
    }
}

impl<T: ReferenceData> Clone for BasicReference<T> {
    fn clone(&self) -> Self {
        Self(self.0.clone())
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
    fn repository() {
        let mut r = Reference::new("foo");
        assert_eq!(r.repostitory(), "foo");
        r = Reference::new("foo:bar");
        assert_eq!(r.repostitory(), "foo");
        r = Reference::new("foo@sha256:baz");
        assert_eq!(r.repostitory(), "foo");
    }
}
