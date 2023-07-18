
use serde_json::Value as Json;

/// Client for connecting to container registries
pub struct Client{
    client: reqwest::Client,
}

impl Client{

    pub fn new() -> Client{
        let mut b = reqwest::Client::builder();
        Client{client: reqwest::Client::new()}
    }

    pub fn manifest_list(&self, ref_: &cent_core::ReferenceView) -> cent_core::ManifestList {
        let _resp = self.client.get(ref_.manifest_url());
        cent_core::ManifestList::new(Json::Null).unwrap()
    }
}
