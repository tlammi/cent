
use serde_json::Value as Json;
use log::*;

/// Client for connecting to container registries
pub struct Client{
    client: reqwest::blocking::Client,
}

impl Client{

    pub fn new() -> Client{
        let mut b = reqwest::Client::builder();
        Client{client: reqwest::blocking::Client::new()}
    }

    pub fn manifest_list(&self, ref_: &cent_core::ReferenceView) -> cent_core::ManifestList {
        let url = ref_.manifest_url();
        trace!("fetching manifest list from {}", url);
        let _resp = self.client.get(url).send();
        trace!("manifest list resp: {:?}", _resp);
        cent_core::ManifestList::new(Json::Null).unwrap()
    }
}
