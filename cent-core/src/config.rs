
use ini::Ini;

/// Configurations
pub struct Config {
    pub root: String,
}

const GLOBAL_CONFIG: &'static str = "/etc/cent/cent.conf";

impl Config {

    pub fn new() -> Self {
        Self{root: ".".to_string()}
    }

    /// Accumulate configs from the configuration files in the system
    pub fn from_config_files() -> Self {
        match Self::from_config_files_impl() {
            Some(r) => r,
            None => Self::new(),
        }
    }

    pub fn from_config_files_impl() -> Option<Self> {
        let conf = Ini::load_from_file(GLOBAL_CONFIG).ok()?;
        let section = conf.section(None::<String>)?;
        let root = section.get("root")?.to_string();
        Some(Self{root})
    }
}

impl Default for Config {
    fn default() -> Self {
        Self::new()
    }
}
