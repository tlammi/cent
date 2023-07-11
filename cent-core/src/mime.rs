use std::marker::PhantomData;
use std::fmt::Debug;


pub trait MimeData: AsRef<str> + Debug + Into<String>{}
impl<T: AsRef<str> + Debug + Into<String>> MimeData for T{}

pub struct BasicMime<'a, T: 'a>{
    v: T,
    phantom: PhantomData<&'a T>,
}


pub type Mime<'a> = BasicMime<'a, String>;
pub type MimeView<'a> = BasicMime<'a, &'a str>;

impl<'a, T: 'a> BasicMime<'a, T> where T: MimeData {
    pub fn new(v: impl Into<T>) -> BasicMime<'a, T>{
        BasicMime { v: v.into(), phantom: PhantomData }
    }

    pub fn toptype(&self) -> &str{
        let v = self.v.as_ref();
        match v.find('/') {
            Some(i) => &v[0..i],
            None => &v,
       }
    }

    pub fn subtype(&self) -> &str {
        let v = self.v.as_ref();
        match v.find('/') {
            Some(i) => &v[i+1..],
            None => "",
        }
    }
}

impl<'a, T: 'a> Debug for BasicMime<'a, T> where T: MimeData {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.v.fmt(f)
    }
}

impl<'a, T: 'a> AsRef<str> for BasicMime<'a, T> where T: MimeData {
    fn as_ref(&self) -> &str {
        self.v.as_ref()
    }
}

impl<'a, T: 'a, U> PartialEq<U> for BasicMime<'a, T> where T: MimeData, U: AsRef<str> {
    fn eq(&self, other: &U) -> bool {
        self.as_ref() == other.as_ref()
    }
}

#[cfg(test)]
mod tests{
    use super::*;

    #[test]
    fn new_mime(){
        let m = Mime::new("foo/bar");
        assert_eq!(m.toptype(), "foo");
        assert_eq!(m.subtype(), "bar");
    }

    #[test]
    fn new_mime_view(){
        let m = MimeView::new("foo/bar");
        assert_eq!(m.toptype(), "foo");
        assert_eq!(m.subtype(), "bar");
    }
}
