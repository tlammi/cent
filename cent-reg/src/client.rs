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

struct Auth {
    /// Where the token was fetched from
    url: reqwest::Url,
    /// Authentication token
    token: String,
}

/// Client for connecting to container registries
pub struct Client {
    client: reqwest::blocking::Client,
    auths: Vec<Auth>,
}

impl Client {
    pub fn new() -> Client {
        let mut b = reqwest::Client::builder();
        Client {
            client: reqwest::blocking::Client::new(),
            auths: vec![],
        }
    }

    pub fn manifest_list(&mut self, ref_: &cent_core::ReferenceView) -> cent_core::ManifestList {
        let url = ref_.manifest_url();
        let mut builder = self.client.get(&url);
        builder = builder.header("Accept", cent_core::mime::DOCKER_MANIFEST_LIST.to_string());
        let mut resp = builder.try_clone().unwrap().send().unwrap();

        match resp.status() {
            reqwest::StatusCode::UNAUTHORIZED => {
                let challenge = resp.headers()["www-authenticate"].as_bytes();
                let auth = self.resolve_auth_from_challenge(challenge);
                builder = builder.header("Authorization", format!("Bearer {}", auth));
                resp = builder.send().unwrap();
            }
            reqwest::StatusCode::OK => (),
            c => panic!("Unsupported HTTP status code {}", c),
        };
        trace!("manifest list resp: {:?}", resp);
        let body = resp.text().unwrap();
        let mut val: Json = serde_json::from_str(&body).unwrap();
        serde_json::from_value(val["manifests"].take()).unwrap()
    }

    pub fn manifest(&mut self, ref_: &cent_core::ReferenceView) -> cent_core::Manifest {
        let url = ref_.manifest_url();
        let mut builder = self.client.get(&url);
        builder = builder.header("Accept", cent_core::mime::DOCKER_MANIFEST.to_string());
        let mut resp = builder.try_clone().unwrap().send().unwrap();

        match resp.status() {
            reqwest::StatusCode::UNAUTHORIZED => {
                let challenge = resp.headers()["www-authenticate"].as_bytes();
                let auth = self.resolve_auth_from_challenge(challenge);
                builder = builder.header("Authorization", format!("Bearer {}", auth));
                resp = builder.send().unwrap();
            }
            reqwest::StatusCode::OK => (),
            c => panic!("Unsupported HTTP status code {}", c),
        };
        trace!("manifest resp: {:?}", resp);
        let body = resp.text().unwrap();
        trace!("manifest body: {}", body);
        let mut val: Json = serde_json::from_str(&body).unwrap();
        cent_core::Manifest::new()
    }

    fn resolve_auth(&mut self, url: reqwest::Url) -> &String {
        let pos = self.auths.iter().position(|auth| auth.url == url);

        match pos {
            Some(i) => return &self.auths[i].token,
            None => {
                let challenge_resp = self.client.get(url.clone()).send();
                let body = challenge_resp.unwrap().text().unwrap();
                let token = serde_json::from_str::<Json>(&body).unwrap()["token"]
                    .as_str()
                    .unwrap()
                    .to_string();
                self.auths.push(Auth { url, token });
                &self.auths.last().unwrap().token
            }
        }
    }

    fn resolve_auth_from_challenge(&mut self, challenge: &[u8]) -> &String {
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
                let url =
                    reqwest::Url::parse_with_params(std::str::from_utf8(bearer).unwrap(), &params)
                        .unwrap();
                warn!("{}", url);
                self.resolve_auth(url)
            }
            None => panic!("no bearer realm"),
        }
    }
}
