
extern crate clap;

use clap::{Parser, Subcommand};

#[derive(Parser, Debug)]
struct RootArgs{

    #[command(subcommand)]
    cmd: Command,

}

#[derive(Subcommand, Debug)]
enum Command{
    Fetch(FetchArgs),
}

#[derive(Debug, Parser)]
struct FetchArgs{

}

pub fn main(){
    println!("hello, world");
}
