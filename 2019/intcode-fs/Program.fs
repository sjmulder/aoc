open System

type Ref = Imm of int
         | Ind of int64
         | Rel of int

type Op = Add   of Ref * Ref * Ref
        | Mul   of Ref * Ref * Ref
        | In    of Ref
        | Out   of Ref
        | Jnz   of Ref * Ref
        | Jz    of Ref * Ref
        | Lt    of Ref * Ref * Ref
        | Eq    of Ref * Ref * Ref
        | Addbp of Ref
        | Hlt

type Opfn = One of (Ref -> unit)
          | More of (Ref -> Opfn)

type Vm = {
    mutable ic: int64
    mutable bp: int64
    mutable halted: bool
    mem: int64[]
}

let deref vm ref =
    match ref with
    | Imm loc -> loc
    | Ind loc -> int vm.mem.[int loc]
    | Rel loc -> int vm.mem.[int vm.bp + loc]

let read  vm ref   = vm.mem.[deref vm ref]
let write vm ref x = vm.mem.[deref vm ref] <- x

let fromMode digit =
    match digit with
    | 0 -> Imm << int
    | 1 -> Ind
    | 2 -> Rel << int
    | _ -> failwithf "Unknown mode: %d" digit

let exec vm op =
    let read  = read  vm
    let write = write vm
    match op with
    | Add   (a, b, c) -> write c <| read a + read b
    | Mul   (a, b, c) -> write c <| read a * read b
    | In    (a)       -> write a << Int64.Parse <| Console.ReadLine()
    | Out   (a)       -> printfn "%d" <| read a
    | Jnz   (a, b)    -> if read a <> (int64 0) then vm.ic <- read b
    | Jz    (a, b)    -> if read a =  (int64 0) then vm.ic <- read b
    | Lt    (a, b, c) -> write c <| if read a < read b then int64 1 else int64 0
    | Eq    (a, b, c) -> write c <| if read a = read b then int64 1 else int64 0
    | Addbp (a)       -> vm.bp <- read a
    | Hlt             -> vm.halted <- true

[<EntryPoint>]
let main argv =
    printfn "Hello World from F#!"
    0
