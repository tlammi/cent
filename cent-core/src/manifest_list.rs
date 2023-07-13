use serde_json::Value;

pub struct ManifestList {
    jsn: Value,
}

impl ManifestList {
    pub fn new(jsn: Value) -> Option<ManifestList> {
        Some(ManifestList { jsn })
    }
}
