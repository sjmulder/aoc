import java.io.File

fun toValue(l: String, p2: Boolean): Int {
    val dig = l.indices
        .mapNotNull {
            when {
                l[it].isDigit() -> l[it]
                p2 && l.startsWith("zero", it) -> '0'
                p2 && l.startsWith("one", it) -> '1'
                p2 && l.startsWith("two", it) -> '2'
                p2 && l.startsWith("three", it) -> '3'
                p2 && l.startsWith("four", it) -> '4'
                p2 && l.startsWith("five", it) -> '5'
                p2 && l.startsWith("six", it) -> '6'
                p2 && l.startsWith("seven", it) -> '7'
                p2 && l.startsWith("eight", it) -> '8'
                p2 && l.startsWith("nine", it) -> '9'
                else -> null
            }
        }
        .toList()

    return ("" + dig.first() + dig.last()).toInt()
}

fun main(args: Array<String>) {
    val lines = File(args.getOrNull(0) ?: "../../inputs/2023/01-input.txt").readLines()
    val p1 = lines.sumOf { toValue(it, false) }
    val p2 = lines.sumOf { toValue(it, true) }
    println("01: $p1 $p2")
}
