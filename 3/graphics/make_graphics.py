import matplotlib.pyplot as plt
import numpy as np
import time
import threading
import random

class MockEntity:
    def __init__(self, id):
        self.id = id
        self.alive = True
        self.time_points = 10
        self.x = random.randint(0, 100)
        self.y = random.randint(0, 100)

    def update(self):
        time.sleep(0.001)
        self.time_points -= 1

    def is_alive(self):
        return self.alive and self.time_points > 0

def process_entities_single_thread(entities):
    for entity in entities:
        if entity.is_alive():
            entity.update()

def process_entities_multi_thread(entities, num_threads=4):
    threads = []
    chunk_size = len(entities) // num_threads

    for i in range(num_threads):
        start_idx = i * chunk_size
        end_idx = start_idx + chunk_size if i < num_threads - 1 else len(entities)
        chunk = entities[start_idx:end_idx]

        thread = threading.Thread(target=process_entities_single_thread, args=(chunk,))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

def run_experiment(entity_counts, use_multithreading=True, num_threads=4):
    times = []

    for count in entity_counts:
        entities = [MockEntity(i) for i in range(count)]

        start_time = time.time()

        if use_multithreading:
            process_entities_multi_thread(entities, num_threads)
        else:
            process_entities_single_thread(entities)

        end_time = time.time()
        times.append(end_time - start_time)

        print(f"Сущностей: {count}, Время: {times[-1]:.3f} сек")

    return times

def plot_comparison(entity_counts, single_thread_times, multi_thread_times):
    plt.figure(figsize=(12, 8))

    plt.plot(entity_counts, single_thread_times,
             color='red',
             linewidth=2,
             marker='o',
             markersize=8,
             label='Без многопоточности')

    plt.plot(entity_counts, multi_thread_times,
             color='blue',
             linewidth=2,
             marker='s',
             markersize=8,
             label='С многопоточностью (4 потока)')

    plt.title('Сравнение производительности обработки сущностей', fontsize=16, fontweight='bold')
    plt.xlabel('Количество сущностей', fontsize=14)
    plt.ylabel('Время обработки (сек)', fontsize=14)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=12)

    for i, (st, mt) in enumerate(zip(single_thread_times, multi_thread_times)):
        speedup = st / mt if mt > 0 else 1
        plt.annotate(f'x{speedup:.1f}',
                    xy=(entity_counts[i], mt),
                    xytext=(0, 10),
                    textcoords='offset points',
                    ha='center',
                    fontsize=9,
                    color='green' if speedup > 1 else 'red')

    plt.tight_layout()
    plt.savefig('performance_comparison.png', dpi=150, bbox_inches='tight')
    plt.close()

def main():
    entity_counts = [10, 50, 100, 200, 500, 1000]

    print("Запуск теста БЕЗ многопоточности...")
    single_thread_times = run_experiment(entity_counts, use_multithreading=False)

    print("\nЗапуск теста С многопоточностью...")
    multi_thread_times = run_experiment(entity_counts, use_multithreading=True, num_threads=4)

    print("\n" + "="*50)
    print("Результаты сравнения:")
    print("="*50)
    for i, count in enumerate(entity_counts):
        speedup = single_thread_times[i] / multi_thread_times[i]
        print(f"{count:4d} сущностей: {single_thread_times[i]:.3f} сек → {multi_thread_times[i]:.3f} сек "
              f"(ускорение: {speedup:.2f}x)")

    plot_comparison(entity_counts, single_thread_times, multi_thread_times)

    with open('performance_results.csv', 'w') as f:
        f.write('EntityCount,SingleThreadTime,MultiThreadTime,Speedup\n')
        for i, count in enumerate(entity_counts):
            speedup = single_thread_times[i] / multi_thread_times[i]
            f.write(f'{count},{single_thread_times[i]:.3f},{multi_thread_times[i]:.3f},{speedup:.2f}\n')

    print("\nДанные сохранены в 'performance_results.csv'")

if __name__ == "__main__":
    main()
