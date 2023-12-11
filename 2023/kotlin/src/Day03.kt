import java.io.File

fun main(args: Array<String>) {
    val lines = File(args.getOrNull(0) ?: "../../inputs/2023/03-input.txt")
        .readLines()

    val gearNums: MutableMap<Int, MutableList<Int>> = mutableMapOf()

    var p1 = 0

    lines.withIndex().forEach { (y, line) ->
        var num = 0
        var sym = false
        val gears: MutableSet<Int> = mutableSetOf()

        // add '.' to end so we don't have to deal with edge case of the last
        // character being a digit
        "$line.".withIndex().forEach { (x, c) ->
            if (c.isDigit()) {
                num = num * 10 + c.toString().toInt()
                sym = sym || (y - 1..y + 1)
                    .filter { it >= 0 && it < lines.count() }
                    .any { y2 ->
                        (x - 1..x + 1)
                            .filter { it >= 0 && it < line.length }
                            .map { lines[y2][it] }
                            .any { it != '.' && !it.isDigit() }
                    }
                gears.addAll((y - 1..y + 1)
                    .filter { it >= 0 && it < lines.count() }
                    .flatMap { y2 -> (x - 1..x + 1)
                        .filter { it >= 0 && it < line.length }
                        .filter { lines[y2][it] == '*' }
                        .map { y2 * 10000 + it } // sue me
                    })
            } else if (num > 0) {
                if (sym) p1 += num
                gears.forEach {
                    val ent = gearNums[it] ?: mutableListOf()
                    ent.add(num)
                    gearNums[it] = ent
                }
                num = 0
                sym = false
                gears.clear()
            }
        }
    }

    val p2: Int = gearNums.values
        .filter { it.count() == 2 }
        .sumOf { it[0] * it[1] }

    println("03: $p1 $p2")
}
