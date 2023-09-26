def sum_two_nums(num1: list[int], num2: list[int]) -> list[int]:
    # супер решение отвечаю!
    n1 = n2 = n3 = 0
    if len(num1) < len(num2):  # костыли обожаю просто вау
        num2, num1 = num1[:], num2[:]

    for i in range(len(num1) - 1, -1, -1):
        n1 += num1[i] * 10 ** (len(num1) - 1 - i)
    for i in range(len(num2) - 1, -1, -1):
        n2 += num2[i] * 10 ** (len(num2) - 1 - i)
    n3 = n1 + n2
    for i in range(len(num1) - 1, -1, -1):  # тут прикольно ржака конеч но но ну и ладно
        num1[i] = (n3 % 10 ** (i + 1)) // 10 ** i
    return num1[::-1]


def merge_intervals(intervals: list[list[int]]) -> list[list[int]]:
    lines = sorted(intervals, key=(lambda x: (x[0], x[1])))
    i = 0
    da_bigga_end = lines[0][1]  # конец 1го элемента чоб сравниват

    while (i + 1) < len(lines):
        if lines[i + 1][0] <= da_bigga_end:  # челы пересекаются лоооол
            da_bigga_end = max(lines[i + 1][1], da_bigga_end)
            low = lines[i][0]
            lines.pop(i)
            lines.pop(i)
            lines.insert(i, [low, da_bigga_end])
        else:  # все ок
            da_bigga_end = max(lines[i + 1][1], da_bigga_end)
            i += 1
    return lines


def remove_duplicates(nums: list[int]) -> None:
    i = 0
    while (i + 1) < len(nums):
        if nums[i + 1] == nums[i]:
            nums.pop(i + 1)
        else:
            i += 1


def compute_unique_pathes(grid: list[list[int]]) -> int:
    path_grid = [[0 for _ in range(3)] for _ in range(3)]
    path_grid[0][0] = 1
    for y in range(len(grid)):
        for x in range(len(grid[0])):
            if grid[y][x] != 1:
                if y > 0 and grid[y - 1][x] != 1:
                    path_grid[y][x] += path_grid[y - 1][x]
                if x > 0 and grid[y][x - 1] != 1:
                    path_grid[y][x] += path_grid[y][x - 1]
    return path_grid[-1][-1]


def jump(jumps: list[int]) -> int:
    jumps_amount = [float('inf') for _ in range(len(jumps))]
    jumps_amount[0] = 0  # НУ МЫ ЖЕ ПРЫГАЕМ ОТКУДА-то
    prev_mx_dist = 1  # предыдущая макс дистанция, на которую могли прыгнуть
    # все тогда нам не нужно рассматривать все прыжки находящиеся в этой области тк можем на ход раньше попасть
    # ну мне лень обхяснять сорян
    for i in range(len(jumps)):
        for x in range(prev_mx_dist, min(i + jumps[i] + 1, len(jumps))):
            jumps_amount[x] = min(jumps_amount[x], jumps_amount[i] + 1)
        prev_mx_dist = min(i + jumps[i], len(jumps)-1)
    return jumps_amount[-1] if jumps_amount[-1] != float('inf') else -1
