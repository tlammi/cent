use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
pub struct Platform {
    pub architecture: String,
    pub os: String,
    pub variant: Option<String>,
}

impl Platform {
    pub fn from_arch(arch: String) -> Platform {
        Platform {
            architecture: arch,
            os: "linux".to_string(),
            variant: None,
        }
    }

    pub fn current_platform() -> Platform {
        use std::env::consts::*;
        let arch = Self::to_container_arch(ARCH);
        let os = OS;
        Platform {
            architecture: arch.to_string(),
            os: os.to_string(),
            variant: None,
        }
    }

    fn to_container_arch(s: &str) -> &'static str {
        if s == "x86_64" {
            return "amd64";
        }
        panic!("Unknown arch: {}", s);
    }
}
