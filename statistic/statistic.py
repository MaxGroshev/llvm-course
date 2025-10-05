from collections import Counter
import sys

def read_data_from_file(filename):
    try:
        with open(filename, 'r', encoding='utf-8') as file:
            data = [line.strip() for line in file if line.strip()]
        return data
    except FileNotFoundError:
        print(f"Error'{filename}' file not found")
        return []
    except Exception as e:
        print(f"Error: {e}")
        return []

def analyze_sequences(data, max_window_size=5):
    results = {}
    
    for window_size in range(1, max_window_size + 1):
        sequences = []
        
        # Создаем скользящие окна указанного размера
        for i in range(len(data) - window_size + 1):
            window = tuple(data[i:i + window_size])
            sequences.append(window)
        
        # Считаем частоту последовательностей
        counter = Counter(sequences)
        
        # Получаем 3 самых частых последовательностей
        most_common = counter.most_common(3)
        results[window_size] = most_common
    
    return results

def print_results(results):
    for window_size, sequences in results.items():
        print(f"\nWindow size: {window_size}")
        print("Top 3")
        
        if not sequences:
            print("  No data")
            continue
            
        for i, (seq, count) in enumerate(sequences, 1):
            seq_str = " → ".join(seq)
            print(f"  {i}. {seq_str} - {count} times")

def main():
    if len(sys.argv) > 1:
        filename = sys.argv[1]
        print(sys.argv[1])
    else:
        filename = input("Input filename: ")
    
    # Читаем данные из файла
    data = read_data_from_file(filename)
    
    if not data:
        print("No data")
        return
    
    print(f"Readen {len(data)} there are '{filename}' lines")
    
    # Анализируем последовательности
    results = analyze_sequences(data)
    
    # Выводим результаты
    print_results(results)

if __name__ == "__main__":
    main()