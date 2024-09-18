import numpy as np
import random
import copy
import matplotlib.pyplot as plt
import math
import folium
import pandas as pd
from tqdm import tqdm
import time


class City:
    def __init__(self, x, y):
        self.lon = x
        self.lat = y

    def __repr__(self):
        return "(" + str(self.lon) + "," + str(self.lat) + ")"


def plot_route_on_map(city_list, route):
    map_center = [np.mean([city.lat for city in city_list]), np.mean([city.lon for city in city_list])]

    my_map = folium.Map(location=map_center, zoom_start=10, tiles=None)
    folium.TileLayer(
        tiles='http://webrd02.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=8&x={x}&y={y}&z={z}',
        attr="&copy; <a href=http://ditu.amap.com/>高德地图</a>",
        min_zoom=0,
        max_zoom=19,
        control=True,
        show=True,
        overlay=False,
        ).add_to(my_map)

    for city in city_list:
        folium.CircleMarker(
            location=[city.lat, city.lon],
            radius=5,
            color='red',
            fill=True,
            fill_color='red',
            fill_opacity=0.6,
            popup=str(city),
        ).add_to(my_map)

    route_coordinates = [[city.lat, city.lon] for city in route] + [[route[0].lat, route[0].lon]]
    folium.PolyLine(locations=route_coordinates, color='blue').add_to(my_map)

    return my_map


# 欧氏距离求距离
def distance_euler(city_1, city_2):
    dx = abs(city_1.lon - city_2.lon)
    dy = abs(city_1.lat - city_2.lat)
    distance = np.sqrt((dx ** 2) + (dy ** 2))
    return distance


# Haversine公式求距离（球面）
def distance_haversine(city_1, city_2):
    lat1, lon1, lat2, lon2 = city_1.lat, city_1.lon, city_2.lat, city_2.lon
    # 将经纬度转换为弧度
    lat1, lon1, lat2, lon2 = map(math.radians, [lat1, lon1, lat2, lon2])

    # Haversine公式
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    a = math.sin(dlat / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

    # 地球半径（单位：千米）
    radius = 6371.0

    # 计算距离
    distance = radius * c
    return distance


# 计算巡回路径长度
def distance_all_citys(population, method='Haversine'):
    temp_dis = 0.0
    if method == 'Haversine':
        for i in range(len(population) - 1):
            temp_dis += distance_haversine(population[i], population[i + 1])
        temp_dis += distance_haversine(population[len(population) - 1], population[0])

    elif method == 'Euler':
        for i in range(len(population) - 1):
            temp_dis += distance_euler(population[i], population[i + 1])
        temp_dis += distance_euler(population[len(population) - 1], population[0])

    return temp_dis

# 适应度函数
def fitness(population, method='Haversine'):
    dis_citys = distance_all_citys(population, method=method)
    return 1.0 / dis_citys


# 种群初始化
def init_pop(city_location, population_size):
    population = []
    for i in range(population_size):
        new_city_list = random.sample(city_location, len(city_location))
        population.append(new_city_list)

    return population


# 适应度排序
def rank(population, method='Haversine'):
    rank_dic = {}
    for i in range(len(population)):
        fit = fitness(population[i], method=method)
        rank_dic[i] = fit

    return sorted(rank_dic.items(), key=lambda x: x[1], reverse=True)


# # 选择操作（锦标赛）
# def select(population, pop_rank, tournament_size):
#     selected_population = []
#
#     # 进行锦标赛选择
#     for i in range(len(population)):
#         # 随机选择锦标赛中的个体
#         tournament_contestants = random.sample(population, tournament_size)
#
#         # 选择锦标赛中适应度最高的个体
#         winner = max(tournament_contestants, key=lambda x: fitness(x))  # 假设有一个名为 fitness 的函数用于计算适应度
#         selected_population.append(winner)
#
#     return selected_population

# 选择操作（精英保留+轮盘赌）
def select(population, pop_rank, elite_size):
    select_population = []

    # 选择精英个体
    for i in range(elite_size):
        select_population.append(population[pop_rank[i][0]])

    cumsum = 0
    cumsum_list = []
    temp_population = copy.deepcopy(pop_rank)

    # 计算适应度的累积和
    for i in range(len(temp_population)):
        cumsum += temp_population[i][1]
        cumsum_list.append(cumsum)

    # 归一化适应度的累积和
    for i in range(len(temp_population)):
        cumsum_list[i] /= cumsum

    # 根据轮盘赌选择非精英个体
    for i in range(len(temp_population) - elite_size):
        rate = random.random()
        for j in range(len(temp_population)):
            if cumsum_list[j] > rate:
                select_population.append(population[pop_rank[i][0]])
                break

    return select_population


# 繁殖操作
def breed(population, elite_size):
    breed_pop = []
    for i in range(elite_size):
        breed_pop.append(population[i])

    i = 0
    while i < (len(population) - elite_size):
        a = random.randint(0, len(population) - 1)
        b = random.randint(0, len(population) - 1)
        if a != b:
            fa, fb = population[a], population[b]
            gene_a, gene_b = random.randint(0, len(population[a]) - 1), random.randint(0, len(population[b]) - 1)
            startgene = min(gene_a, gene_b)
            endgene = max(gene_a, gene_b)
            child1 = []
            for j in range(startgene, endgene):
                child1.append(fa[j])
            # child1 = copy.deepcopy(fa[:-1])
            child2 = []
            for j in fb:
                if j not in child1:
                    child2.append(j)
            # child2 = [j for j in fb if j not in child1]
            breed_pop.append(child1 + child2)
            i = i + 1

    return breed_pop


# 交叉操作
def crossover(population, crossover_rate):
    crossover_population = []
    size = len(population)
    start = random.randint(0, size - 1)
    end = random.randint(start + 1, size)

    for i in range(0, len(population), 2):
        # 根据交叉率决定是否进行交叉
        if random.random() < crossover_rate:
            parent1 = population[i]
            parent2 = population[(i + 1) % size]

            child1 = [None] * len(parent1)
            child2 = [None] * len(parent2)

            # 复制父代基因段到子代
            child1[start:end] = parent1[start:end]
            child2[start:end] = parent2[start:end]

            j = end % len(parent1)
            for city in parent2:
                if parent1[j] not in child1:
                    child1[j] = parent1[j]
                j = (j + 1) % len(parent1)

            j = end % len(parent2)
            for city in parent1:
                if parent2[j] not in child2:
                    child2[j] = parent2[j]
                j = (j + 1) % len(parent2)

            # 将新生成的子代添加到交叉种群中
            crossover_population.append(child1)
            crossover_population.append(child2)
        else:
            # 如果不进行交叉，则直接将父代添加到交叉种群中
            crossover_population.append(population[i])
            crossover_population.append(population[(i + 1) % size])

    return crossover_population


# 变异操作
def mutate(population, mutation_rate):
    mutation_population = []
    for i in range(len(population)):
        for j in range(len(population[i])):
            # 生成随机数判断是否进行变异
            rate = random.random()
            if rate < mutation_rate:
                # 如果满足变异率，随机选择两个基因位置并交换它们
                a = random.randint(0, len(population[i]) - 1)
                population[i][a], population[i][j] = population[i][j], population[i][a]

            # 将发生变异的个体添加到变异种群中
        mutation_population.append(population[i])

    return mutation_population


# 生成下一代种群
def generate_next(population, elite_size, crossover_rate, mutation_rate, method='Haversine'):
    pop_rank = rank(population, method=method)  # 按照适应度排序
    select_pop = select(population, pop_rank, elite_size)  # 精英选择策略，加上轮盘赌选择
    breed_pop = breed(select_pop, elite_size)  # 繁殖
    # cross_pop = crossover(select_pop, crossover_rate)  # 交叉
    cross_pop = crossover(breed_pop, crossover_rate)  # 交叉
    next_generation = mutate(cross_pop, mutation_rate)  # 变异

    return next_generation


# 绘制每次迭代的过程
def plot_each(city_list, population_size, elite_size, crossover_rate, mutation_rate, generations, method='Haversine'):
    plt.figure()
    plt.ion()
    population = init_pop(city_list, population_size)

    print("初始距离:{}".format(1.0 / (rank(population, method=method)[0][1])))
    for i in range(generations):
        plt.cla()
        population = generate_next(population, elite_size, crossover_rate, mutation_rate, method=method)
        idx_rank_pop = rank(population, method=method)[0][0]
        best_route = population[idx_rank_pop]
        city_x = []
        city_y = []
        for j in range(len(best_route)):
            city = best_route[j]
            city_x.append(city.lon)
            city_y.append(city.lat)
        city_x.append(best_route[0].lon)
        city_y.append(best_route[0].lat)
        plt.scatter(city_x, city_y, c='r', marker='o', s=100, alpha=0.5)

        plt.title('Current distance='+str(1.0 / (rank(population, method=method)[0][1])))
        plt.plot(city_x, city_y, "k", ms=20)
        plt.pause(0.1)

    plt.ioff()
    plt.show()

    print("最终距离:{}".format(1.0 / (rank(population, method=method)[0][1])))
    bestRouteIndex = rank(population, method=method)[0][0]
    bestRoute = population[bestRouteIndex]


# 绘制最后的结果
def plot_final(city_location, population_size, elite_size, crossover_rate, mutation_rate, generations, method='Haversine'):
    population = init_pop(city_location, population_size)  # 初始化种群
    print("初始距离: {}".format(1.0 / (rank(population, method=method)[0][1])))
    process = []
    time.sleep(1)
    for i in tqdm(range(generations), desc="Searching Progress"):
        population = generate_next(population, elite_size, crossover_rate, mutation_rate, method=method)  # 产生下一代种群
        process.append(1.0 / (rank(population, method=method)[0][1]))

    print("最终距离: {}".format(1.0 / (rank(population, method=method)[0][1])))
    plt.figure(1)
    plt.plot(process)
    plt.ylabel('Distance(km)')
    plt.xlabel('Generations')
    plt.savefig(str(population_size) + '_' + str(elite_size) + '_' + str(crossover_rate) + '_' + str(mutation_rate) + '_' + str(generations) + str(method) + '.jpg')
    plt.show()

    series_data = pd.Series(process)
    df = pd.DataFrame({'Data': series_data})
    df.to_excel(str(population_size) + '_' + str(elite_size) + '_' + str(crossover_rate) + '_' + str(mutation_rate) + '_' + str(generations) + str(method) + '.xlsx', index=False, header=False)

    plt.figure(2)
    idx_rank_pop = rank(population, method=method)[0][0]
    best_route = population[idx_rank_pop]
    my_map = plot_route_on_map(city_location, best_route)

    my_map.save(str(population_size) + '_' + str(elite_size) + '_' + str(crossover_rate) + '_' + str(mutation_rate) + '_' + str(generations) + str(method) + '_route.html')
    print("地图已保存 ")


if __name__ == "__main__":
    city_location = []  # 存储城市坐标

    with open('city.txt', 'r', encoding='UTF-8') as f:
        lines = f.readlines()

    for line in lines:
        line = line.replace('\n', '')
        location = line.split('\t')
        city_location.append(City(float(location[1]), float(location[2])))

    # 绘制最终迭代结果
    plot_final(city_location, 100, 20, 0.5, 0.01, 500, method='Euler')   # (城市经纬度数据, 种群大小, 精英数量, 交叉概率, 变异概率, 进化代数, 求距离方法), method = Haversine or Euler

    # 绘制迭代过程
    # plot_each(city_location, 100, 20, 0.5, 0.01, 500, method='Euler')
