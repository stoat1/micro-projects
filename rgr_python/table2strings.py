# transform table (matrix) to strings

def __getCols(table):
    cols = 0
    for row in table:
        cols = max(cols, len(row))
    return cols

def __getHorizontalLine(colsWidth):
    result = "+"
    for w in colsWidth:
        result += "-" * w + "+"
    return result

def table2strings(table):
    cols = __getCols(table)
    
    stringTable = []
    for row in table:
        stringRow = []
        for elem in row:
            stringRow.append(str(elem))
        stringTable.append(stringRow)

    for stringRow in stringTable:
        while len(stringRow) < cols:
            stringRow.append("")

    colsWidth = []
    for j in range(cols):
        maxLength = 0
        for stringRow in stringTable:
            maxLength = max(maxLength, len(stringRow[j]))
        for stringRow in stringTable:
            while len( stringRow[j] ) < maxLength:
                stringRow[j] += " "
        colsWidth.append(maxLength)

    hLine = __getHorizontalLine(colsWidth)
    result = [hLine]
    for stringRow in stringTable:
        string = "|"
        for s in stringRow:
            string += s + "|"
        result.append(string)
        result.append(hLine)

    return result

def print_table(table):
    for s in table2strings(table):
        print(s)

table = [
    ["N", 1, 2, 3],
    ["first", 0, 0, 0],
    ["second"],
    ["third", 0, 0, 0],
    ["fore", 0, 0, 0]
]
