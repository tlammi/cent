use serde_json::Value as Json;

pub struct ManifestList {
    jsn: Json,
}

impl ManifestList {
    pub fn new(jsn: Json) -> Option<ManifestList> {
        Some(ManifestList { jsn })
    }
}
