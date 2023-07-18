extern crate clap;

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
    /// Fetch resources
    Fetch(FetchArgs),
}

#[derive(Debug, Parser)]
struct FetchArgs {
    /// Resource type to fetch
    what: What,
    /// Reference to the resource, e.g. "docker.io/library/alpine:latest"
    reference: String,
}

#[derive(Debug, ValueEnum, Clone, PartialEq)]
enum What {
    /// Fetch manifest list
    ManifestList
}

fn fetch(args: FetchArgs){
    if args.what != What::ManifestList {
        panic!("asdfasf");
    }
    let ref_ = cent::core::Reference::new(args.reference);
    let client = cent::reg::Client::new();
    client.manifest_list(&ref_.view());
}


pub fn main() {
    let args = RootArgs::parse();
    SimpleLogger::new().with_level(args.log_level).init().unwrap();
    trace!("CLI: {:?}", args);
    match args.cmd {
        Command::Fetch(f) => fetch(f),
    };
}
