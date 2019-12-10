open System
open System.IO

let dup f a = f a a
let iff t f b = if b then t else f
let fromChars cs = new string(Seq.toArray cs)

let readInput = String.filter (not << Char.IsWhiteSpace) << File.ReadAllText
let toLayers = (>>) Seq.chunkBySize << (>>) << Seq.chunkBySize
let countInLayer = (<<) Seq.sum << Seq.map << (<<) Seq.length << Seq.where << (=)
let fullestLayer = (<<) Seq.minBy countInLayer '0'
let flattenPixels = (<<) Seq.head << Seq.filter <| (<>) '2'
let flattenLayers = (<<) Seq.map Seq.map flattenPixels << Seq.map Seq.transpose << Seq.transpose
let toDisplayChar = iff '#' ' ' << (=) '1'
let toDisplay = String.concat "\r\n" << Seq.map (fromChars << Seq.map toDisplayChar)
let part1Score = (<<) (>>) countInLayer '2' << (*) << countInLayer '1' |> dup
let part1 = part1Score << fullestLayer
let part2 = toDisplay << flattenLayers

[<EntryPoint>]
let main argv =   
    let layers = toLayers 25 6 <| readInput "input"
    printfn "Part 1:%d" <| part1 layers
    printfn "Part 2:\n%s" <| part2 layers
    0