
/// Client for connecting to container registries
pub struct Client{
    client: reqwest::Client,
}

impl Client{

    pub fn new() -> Client{
        Client{client: reqwest::Client::new()}
    }

}
