use ini::Ini;

pub struct Config {
    pub root: String,
    pub run_root: String,
}

const GLOBAL_CONFIG_PATH: &str = "/etc/cent/cent.conf";

impl Config {
    pub fn new() -> Config {
        Config {
            root: "".to_string(),
            run_root: "".to_string(),
        }
    }

    pub fn load_files() -> Config {
        let mut c = Self::new();
        c.load_file(GLOBAL_CONFIG_PATH);
        c
    }

    pub fn load_file(&mut self, path: &str) -> bool {
        match Self::try_load_file(path) {
            Some(c) => {
                *self = c;
                return true;
            }
            None => {
                return false;
            }
        };
    }

    fn try_load_file(path: &str) -> Option<Config> {
        let conf = Ini::load_from_file(path).ok()?;
        let section = conf.section(None::<String>)?;
        let root = section.get("root")?;
        let run_root = section.get("run_root")?;
        Some(Config {
            root: root.to_string(),
            run_root: run_root.to_string(),
        })
    }
}

impl Default for Config {
    fn default() -> Self {
        Self::new()
    }
}

pub struct ConfigBuilder {}

impl ConfigBuilder {
    pub fn new() -> ConfigBuilder {
        ConfigBuilder {}
    }
}
