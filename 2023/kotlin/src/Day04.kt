import java.io.File
import kotlin.math.pow

fun main(args: Array<String>) {
    val matches = File(args.getOrNull(0) ?: "../../inputs/2023/04-input.txt")
        .readLines()
        .map { line -> line.split(':')[1].split('|')
            .map { s -> s.split(' ')
                .filter { it.isNotEmpty() }
                .map { it.toInt() }
                .toSet()
            }
        }
        .map { (wins, nums) -> wins.intersect(nums).count() }

    val counts = mutableMapOf<Int, Int>()

    matches.withIndex().forEach { (i, n) ->
        (0..<n).forEach {
            counts[i+1+it] = (counts[i+1+it] ?: 0) + (counts[i] ?: 0) + 1
        }
    }

    val p1 = matches.sumOf { if (it == 0) 0 else 2.0.pow(it - 1.0).toInt() }
    val p2 = matches.count() + counts.values.sum()

    println("04: $p1 $p2")
}
