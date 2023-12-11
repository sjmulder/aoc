import java.io.File

fun main(args: Array<String>) {
    val games = File(args.getOrNull(0) ?:
            "../../inputs/2023/02-input.txt")
        .readLines()
        .map { line -> line
            .split(':')[1].trim()
            .split(';')
            .map { round ->
                round.split(',')
                    .map { it.trim().split(' ') }
                    .associateBy( { it[1] }, { it[0].toInt() })
            }
        }

    val p1 = games.withIndex()
        .filter { (_, rounds) ->
            rounds.all {
                it.getOrDefault("red", 0) <= 12 &&
                it.getOrDefault("green", 0) <= 13 &&
                it.getOrDefault("blue", 0) <= 14
            }
        }
        .sumOf { it.index + 1 }

    val p2 = games
        .sumOf { rounds ->
            rounds.maxOf { it.getOrDefault("red", 0) } *
            rounds.maxOf { it.getOrDefault("green", 0) } *
            rounds.maxOf { it.getOrDefault("blue", 0) }
        }

    print("02: $p1 $p2")
}
