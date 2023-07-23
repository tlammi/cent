use core::fmt::Debug;
/**
* Type for handling header fields with
* */
use core::iter::zip;

pub struct HeaderView<'a> {
    values: Vec<(&'a [u8], &'a [u8])>,
}

impl<'a> HeaderView<'a> {
    pub fn new(val: &'a [u8]) -> HeaderView<'a> {
        let mut prev_idx = 0;
        let mut bits = Vec::<&'a [u8]>::new();

        val.iter()
            .enumerate()
            .filter(|(_, &c)| c == b',')
            .for_each(|(i, _)| {
                bits.push(&val[prev_idx..i]);
                prev_idx = i+1
            });
        bits.push(&val[prev_idx..]);

        let values = bits
            .iter()
            .map(|&s| {
                let idx: usize = s.iter().position(|&c| c == b'=').unwrap();
                return (&s[..idx], &s[idx + 2..s.len() - 1]);
            })
            .collect();
        HeaderView { values }
    }

    pub fn at(&self, index: &[u8]) -> Option<&[u8]> {
        for (k, v) in &self.values {
            if index.len() != k.len() {
                continue;
            }
            let mut match_found = true;
            for (kc, ic) in zip(*k, index) {
                if kc.to_ascii_lowercase() != ic.to_ascii_lowercase() {
                    match_found = false;
                    break;
                }
            }
            if match_found {
                return Some(&v);
            }
        }
        None
    }
}

impl<'a> Debug for HeaderView<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let vals: Vec<String> = self
            .values
            .iter()
            .map(|(k, v)| {
                "'".to_owned()
                    + std::str::from_utf8(k).unwrap()
                    + "': '"
                    + std::str::from_utf8(v).unwrap()
                    + "'"
            })
            .collect();
        f.debug_struct("HeaderView").field("values", &vals).finish()
    }
}

#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn single_value() {
        let h = HeaderView::new(b"Bearer realm=\"foo/bar\"");
        let res = h.at(b"bearer realm");
        assert!(res.is_some());
        assert_eq!(res.unwrap(), b"foo/bar");
    }

    #[test]
    fn multiple_values(){
        let input = b"Bearer realm=\"https://auth.docker.io/token\",service=\"registry.docker.io\"";
        let h = HeaderView::new(input);
        let bearer = h.at(b"bearer realm").unwrap();
        let service = h.at(b"service").unwrap();
        assert_eq!(bearer, b"https://auth.docker.io/token");
        assert_eq!(service, b"registry.docker.io");
    }
}

