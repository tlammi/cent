use crate::Digest;
use crate::Mime;
use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
pub struct ManifestField {
    #[serde(rename = "mediaType")]
    pub mime: Mime,
    pub size: usize,
    pub digest: Digest,
}

#[derive(Debug, Serialize, Deserialize)]
pub struct Manifest {
    pub config: ManifestField,
    pub layers: Vec<ManifestField>,
}

impl Manifest {
    pub fn new() -> Manifest {
        Manifest {
            config: ManifestField {
                mime: Mime::new(""),
                size: 0,
                digest: Digest::new(""),
            },
            layers: vec![],
        }
    }
}
