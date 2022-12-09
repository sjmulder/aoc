use std::io::stdin;

fn main() {
    let mut top : [u64; 4] = [0; 4];
    let mut group = 0;

    for line in stdin().lines().flatten()  {
        if let Ok(val) = line.parse::<u64>() {
            group += val;
        } else {
            top[0] = group;
            top.sort();
            group = 0;
        }
    }

    println!("01: {} {}",
        top.last().unwrap(),
        top.iter().skip(1).sum::<u64>());
}
