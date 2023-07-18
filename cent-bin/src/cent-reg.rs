extern crate clap;

use clap::{Parser, Subcommand, ValueEnum};
use simple_logger::SimpleLogger;
use log::*;

#[derive(Parser, Debug)]
struct RootArgs {
    #[command(subcommand)]
    cmd: Command,
}

#[derive(Subcommand, Debug)]
enum Command {
    Fetch(FetchArgs),
}

#[derive(Debug, Parser)]
struct FetchArgs {
    /// Resource type to fetch
    what: What,
    /// Reference to the resource, e.g. "docker.io/library/alpine:latest"
    reference: String,
}

#[derive(Debug, ValueEnum, Clone)]
enum What {
    /// Fetch manifest list
    ManifestList
}

pub fn main() {
    SimpleLogger::new().init().unwrap();
    let args = RootArgs::parse();
    trace!("CLI: {:?}", args);
    println!("hello, world");
}
