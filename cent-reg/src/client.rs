use crate::header_view::HeaderView;
use log::*;
use serde_json::Value as Json;

/// Client for connecting to container registries
pub struct Client {
    client: reqwest::blocking::Client,
}

impl Client {
    pub fn new() -> Client {
        let mut b = reqwest::Client::builder();
        Client {
            client: reqwest::blocking::Client::new(),
        }
    }

    pub fn manifest_list(&self, ref_: &cent_core::ReferenceView) -> cent_core::ManifestList {
        let url = ref_.registry_root_url().unwrap();
        let resp = self.client.get(&url).send().unwrap();
        trace!("root url resp: {:?}", resp);

        match resp.status() {
            reqwest::StatusCode::UNAUTHORIZED => {
                let challenge = resp.headers()["www-authenticate"].as_bytes();
                let view = HeaderView::new(challenge);
                match view.at(b"bearer realm") {
                    Some(v) => {
                        info!("bearer realm: {}", std::str::from_utf8(v).unwrap());
                    }
                    None => error!("no bearer realm"),
                }
            }
            _ => (),
        };

        let url = ref_.manifest_url();
        trace!("fetching manifest list from {}", url);
        let _resp = self.client.get(url).send();
        trace!("manifest list resp: {:?}", _resp);
        cent_core::ManifestList::new(Json::Null).unwrap()
    }
}
