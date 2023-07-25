use serde_json::Value as Json;

use crate::Digest;
use crate::Mime;
use crate::Platform;

use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
pub struct ManifestListElem {
    pub digest: Digest,

    #[serde(rename = "mediaType")]
    pub mime: Mime,
    pub platform: Platform,
}

pub type ManifestList = Vec<ManifestListElem>;

