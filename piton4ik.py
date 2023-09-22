def sum_two_nums(num1: list[int], num2: list[int]) -> list[int]:
    # супер решение отвечаю!
    n1 = n2 = n3 = 0
    if (len(num1) < len(num2)): # костыли обожаю просто вау
        num2, num1 = num1[:], num2[:]

    for i in range(len(num1)-1, -1, -1):
        n1 += num1[i]*10**(len(num1) - 1 -i)
    for i in range(len(num2)-1, -1, -1):
        n2 += num2[i]*10**(len(num2) - 1 -i)
    n3 = n1 + n2
    for i in range(len(num1)-1, -1, -1): # тут прикольно ржака конеч но но ну и ладно
        num1[i] = (n3 % 10**(i+1)) // 10**i
    return num1[::-1]


def merge_intervals(intervals: list[list[int]]) -> list[list[int]]:
    intervals.sort(key=(lambda x: x[0]))
    min_start = -1
    index_for_min = -1
    while (intervals):
        tmp_min = 1000000000000000000000000000000000000000000000000
        intervals_tmp = intervals[:]
        for i in range(len(intervals)):
            if intervals[i] > min_start:  # это не мамонт
                intervals_tmp.append(intervals[i][:])
                if (tmp_min > intervals_tmp)


    return []


intervals = [[1,3],[2,6],[8,10],[15,18]]
assert merge_intervals(intervals) == [[1,6],[8,10],[15,18]]
