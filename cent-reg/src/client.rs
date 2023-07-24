use crate::header_view::HeaderView;
use log::*;
use serde_json::Value as Json;
// use serde::Deserialize;
// use chrono::{DateTime, FixedOffset};
// use reqwest::StatusCode;
//
//
// #[derive(Deserialize, Debug)]
// struct BearerTokenResp{
//     access_token: String,
//     token: String,
//     issued_at: DateTime<FixedOffset>,
//     expires_in: usize,
// }

enum Auth {
    Null,
    Bearer(String),
}

/// Client for connecting to container registries
pub struct Client {
    client: reqwest::blocking::Client,
    auth: Auth,
}

impl Client {
    pub fn new() -> Client {
        let mut b = reqwest::Client::builder();
        Client {
            client: reqwest::blocking::Client::new(),
            auth: Auth::Null,
        }
    }

    pub fn manifest_list(&mut self, ref_: &cent_core::ReferenceView) -> cent_core::ManifestList {
        let url = ref_.manifest_url();

        let mut builder = self.client.get(&url);
        builder = builder.header("Accept", cent_core::mime::DOCKER_MANIFEST_LIST.to_string());
        let resp = builder.try_clone().unwrap().send().unwrap();

        match resp.status() {
            reqwest::StatusCode::UNAUTHORIZED => {
                let challenge = resp.headers()["www-authenticate"].as_bytes();
                let view = HeaderView::new(challenge);
                match view.at(b"bearer realm") {
                    Some(bearer) => {
                        warn!("bearer: {:?}", view);
                        let service = view.at(b"service").unwrap_or(b"");
                        let scope = view.at(b"scope").unwrap_or(b"");
                        let params = [
                            ("service", std::str::from_utf8(service).unwrap()),
                            ("scope", std::str::from_utf8(scope).unwrap()),
                        ];
                        let url = reqwest::Url::parse_with_params(
                            std::str::from_utf8(bearer).unwrap(),
                            &params,
                        )
                        .unwrap();
                        warn!("{}", url);
                        let challenge_resp = self.client.get(url).send();
                        let body = challenge_resp.unwrap().text().unwrap();
                        let token = serde_json::from_str::<Json>(&body).unwrap()["token"]
                            .as_str()
                            .unwrap()
                            .to_string();
                        self.auth = Auth::Bearer(token);
                    }
                    None => panic!("no bearer realm"),
                }
            }
            reqwest::StatusCode::OK => (),
            c => panic!("Unsupported HTTP status code {}", c),
        };
        trace!("fetching manifest list from {}", url);
        match &self.auth {
            Auth::Null => (),
            Auth::Bearer(token) => {
                builder = builder.header("Authorization", format!("Bearer {}", token))
            }
        };
        trace!("request: {:?}", builder);
        let _resp = builder.send();
        trace!("manifest list resp: {:?}", _resp);
        let body = _resp.unwrap().text().unwrap();
        serde_json::from_str(&body).unwrap()
    }
}
