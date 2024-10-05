def average_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        
    # N и K
    for i in range(min(2, len(lines))):
        print(lines[i].strip())
    
    lines = lines[2:]

    total = 0
    count = 0

    for line in lines:
        try:
            value = float(line.strip())
            total += value
            count += 1
        except ValueError:
            print(f"Некорректное значение: {line.strip()}")
    
    if count == 0:
        print("Нет значений для расчета среднего.")
        return None
    
    average = total / count
    return average

def main():
    filename = 'resources/outputServer.txt'
    result = average_from_file(filename)

    if result is not None:
        print(f"Среднее значение: {result}")

if __name__ == '__main__':
    main()