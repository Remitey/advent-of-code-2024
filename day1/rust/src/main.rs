use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader};

fn similarityfn(col1: &Vec<i32>, col2: &Vec<i32>) -> i32 {
    let mut similarity = 0;
    let mut counter = 0;
    for element in col1 {
        for element2 in col2 {
            if element == element2 {
                counter += 1;
            }
        }
        similarity += element*counter;
        counter = 0;
    }

    return similarity;
}

fn main() -> io::Result<()> {
    let file_path = env::var("FILE_PATH").expect("FILE PATH is not define");
    let file = File::open(file_path)?;
    let reader = BufReader::new(file);

    let mut col1: Vec<i32> = Vec::new();
    let mut col2: Vec<i32> = Vec::new();
    let mut totaldistance = 0;

    for line in reader.lines() {
        let line = line?;

        let numbers: Vec<i32> = line
            .split_whitespace()
            .filter_map(|s| s.parse().ok())
            .collect();

        if numbers.len() == 2 {
            col1.push(numbers[0]);
            col2.push(numbers[1]);
        }
    }
    #[cfg(feature = "unstable")]
    {
        col1.sort_unstable();
        col2.sort_unstable();
    }
    #[cfg(not(feature = "unstable"))]
    {
        col1.sort();
        col2.sort();
    }
    for i in 0..col1.len() {
        totaldistance += (col1[i] - col2[i]).abs();
    }
    let similarity = similarityfn(&col1, &col2);
    println!("Totaldistance : {}", totaldistance);
    println!("Similarity : {}", similarity);

    Ok(())
}
