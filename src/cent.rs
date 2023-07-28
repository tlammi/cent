use std::io::Write;

use clap::{Parser, Subcommand, ValueEnum};
use simple_logger::SimpleLogger;
//use log::LevelFilter;
use log::*;

#[derive(Parser, Debug)]
struct RootArgs {
    #[arg(short, long, default_value_t = LevelFilter::Warn)]
    log_level: LevelFilter,
    #[command(subcommand)]
    cmd: Command,
}

#[derive(Subcommand, Debug)]
enum Command {
    /// Fetch raw resources
    ///
    /// This can be used to fetch raw resources, such as manifest lists, manifests or layers from
    /// container registries.
    Fetch(FetchArgs),
}

#[derive(Debug, Parser)]
struct FetchArgs {
    /// Output file, or directory if fetch results in multiple files
    #[arg(long)]
    out: Option<String>,
    /// Resource type to fetch
    what: What,
    /// Reference to the resource, e.g. "docker.io/library/alpine:latest"
    reference: String,
}

#[derive(Debug, ValueEnum, Clone, PartialEq)]
enum What {
    /// Fetch manifest list
    ManifestList,
    /// Fetch manifest
    Manifest,
    /// Fetch layers of the image
    Layers,
}

fn fetch(args: FetchArgs) {
    let ref_ = cent::Reference::new(args.reference);
    let mut client = cent::reg::Client::new();

    use What::*;
    match args.what {
        ManifestList => {
            let mlist = client.manifest_list(&ref_.view());
            println!("{}", serde_json::to_string(&mlist).unwrap());
        }
        Manifest => {
            let m = client.manifest(&ref_.view());
            println!("{}", serde_json::to_string(&m).unwrap());
        }
        Layers => {
            let m = client.manifest(&ref_.view());
            let mut imgref = ref_.clone();
            for layer in &m.layers {
                imgref.set_suffix(cent::Suffix::Digest(layer.digest.view()));
                let b = client.blob(&imgref.view(), layer.mime.view());
                let mut f = std::fs::File::create(layer.digest.str()).unwrap();
                f.write_all(b.as_slice()).unwrap();
            }
        }
    }
}

pub fn main() {
    let args = RootArgs::parse();
    SimpleLogger::new()
        .with_level(args.log_level)
        .init()
        .unwrap();
    trace!("CLI: {:?}", args);
    match args.cmd {
        Command::Fetch(f) => fetch(f),
    };
}
