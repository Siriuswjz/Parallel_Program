import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

def plot_speedup():
    """读取CSV文件并绘制加速比折线图"""
    try:
        df = pd.read_csv('performance_results.csv')
        
        threads = df['threads']
        speedups = df['serial_time'] / df['parallel_time']
        
        plt.figure(figsize=(10, 6))
        
        plt.plot(threads, speedups, 'b-o', linewidth=2, markersize=8, label='实际加速比')
        
        # 绘制理想加速比（线性加速）
        ideal_speedup = threads
        plt.plot(threads, ideal_speedup, 'r--', linewidth=2, alpha=0.7, label='理想加速比')
        
        # 设置图表属性
        plt.xlabel('线程数', fontsize=12)
        plt.ylabel('加速比', fontsize=12)
        plt.title('泰勒级数并行计算加速比分析', fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        
        plt.xlim(0, max(threads) + 1)
        plt.ylim(0, max(max(speedups), max(threads)) + 1)
        
        for i, (thread, speedup) in enumerate(zip(threads, speedups)):
            plt.annotate(f'{speedup:.2f}', 
                        (thread, speedup), 
                        textcoords="offset points", 
                        xytext=(0,10), 
                        ha='center',
                        fontsize=9)
        
        # 保存图片
        plt.tight_layout()
        plt.savefig('speedup_chart.png', dpi=300, bbox_inches='tight')
        
        # 显示图表
        plt.show()
        
        # 打印分析结果
        print("\n=== 加速比分析结果 ===")
        print(f"最大加速比: {max(speedups):.2f}x (使用 {threads[speedups.idxmax()]} 个线程)")
        
        # 计算并行效率
        efficiency = speedups / threads
        print(f"最高并行效率: {max(efficiency):.2f} (使用 {threads[efficiency.idxmax()]} 个线程)")
        
        print("\n详细数据:")
        for thread, speedup, eff in zip(threads, speedups, efficiency):
            print(f"线程数: {thread:2d}, 加速比: {speedup:5.2f}, 效率: {eff:5.2%}")
        
        print(f"\n图表已保存为: speedup_chart.png")
        
    except FileNotFoundError:
        print("错误: 找不到 performance_results.csv 文件")
    except Exception as e:
        print(f"错误: {e}")

if __name__ == "__main__":
    plot_speedup()