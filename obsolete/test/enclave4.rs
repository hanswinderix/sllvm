#![no_std]
#![feature(linkage)]
#![feature(custom_attribute)]

static SECRET: i16 = 42;

#[no_mangle]
#[sllvm]
pub fn enclave4_public_get_secret(p1: i16, p2: i16) -> i16 {
    match may_fail(false) {
        Ok(_) => SECRET,
        Err(_) => plus_one(SECRET) + p1 + p2
    }
}

/*
#[no_mangle]
#[sllvm]
pub fn enclave4_public_test1(p1: i16) -> i16 {
    return p1;
}

#[no_mangle]
#[sllvm]
pub fn enclave4_public_test2(p1: i16) -> i16 {
    return p1;
}
*/

#[linkage = "internal"]
fn plus_one(x: i16) -> i16 {
    x + 1
}

#[linkage = "internal"]
//TODO: fn may_fail(failure: bool) -> Result<(), &'static str> { 
fn may_fail(failure: bool) -> Result<(), i16> {
    if failure {
        //Err("this didn’t work!")
        Err(10)
    } else {
        Ok(())
    }
}
