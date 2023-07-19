
/**
* Type for handling header fields with 
* */
pub struct HeaderView<'a>{
    val: &'a [u8] 
}

impl<'a> HeaderView<'a> {

    pub fn new(val: &'a [u8]) -> HeaderView<'a> {
        HeaderView{val}
    }

    pub fn at(&self, index: &[u8]) -> Option<&[u8]> {
        let value = self.val.to_ascii_lowercase();
        let searched = index.to_ascii_lowercase();
        let begin = value.windows(index.len()).position(|window| window == searched)?;
        let end = begin + index.len();
        let eq = value[end..].iter().position(|&v| v == b'=')? + end;
        let comma = value[eq..].iter().position(|&v| v == b',')? + eq;
        return Some(&self.val[eq+2..comma-1]);
    }
}

