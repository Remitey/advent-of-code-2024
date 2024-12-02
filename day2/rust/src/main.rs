use std::fs::File;
use std::io::{self, BufRead, BufReader};

fn is_safe(line: &[i32]) -> bool {
    let size = line.len();
    if size < 2 {
        return true;
    }
    let mut direction = 0;
    for element in 0..(size - 1) {
        let diff = line[element + 1] - line[element];
        if diff.abs() > 3 || diff.abs() < 1 {
            return false;
        }
        if direction == 0 {
            direction = match diff.cmp(&0) {
                std::cmp::Ordering::Greater => 1,
                std::cmp::Ordering::Less => -1,
                std::cmp::Ordering::Equal => 0,
            };
        } else if (direction == 1 && diff < 0) || (direction == -1 && diff > 0) {
            return false;
        }
    }
    true
}

fn safe_line(table: &[Vec<i32>], sizes: &[usize], num_rows: usize) -> i32 {
    let mut safe_line_count = 0;

    for i in 0..num_rows {
        let line = &table[i];
        let size = sizes[i];
        if is_safe(line) {
            safe_line_count += 1;
        } else {
            let mut safe = false;
            for j in 0..size {
                let mut modified_line = line.clone();
                modified_line.remove(j);
                if is_safe(&modified_line) {
                    safe = true;
                    break;
                }
            }
            if safe {
                safe_line_count += 1;
            }
        }
    }

    safe_line_count
}

fn main() -> io::Result<()> {
    let file = File::open("day2/input.txt")?;
    let reader = BufReader::new(file);

    let mut table: Vec<Vec<i32>> = Vec::new();
    let mut sizes: Vec<usize> = Vec::new();

    for line in reader.lines() {
        let line = line?;
        let mut row: Vec<i32> = Vec::new();

        for token in line.split_whitespace() {
            if let Ok(num) = token.parse::<i32>() {
                row.push(num);
            } else {
                eprintln!("Error token: {}", token);
            }
        }
        sizes.push(row.len());
        table.push(row);
    }
    let num_rows = table.len();
    let safe_line_total = safe_line(&table, &sizes, num_rows);
    println!("result {}", safe_line_total);

    Ok(())
}
