import sys
from collections import defaultdict, deque

n = m = l = 0
farthest_neighbor = 0

def read_input():
    global n, m, l
    n, m, l = map(int, input().split())

    if m == 0 and l == 0:
        return -1, None
    
    # Usar sets e dicionários ao invés de matrizes
    main_graph = defaultdict(set)
    station_lines = defaultdict(set)
    working_stations = set()
    lines_per_station = [0] * l
    
    for _ in range(m):
        s1, s2, cl = map(int, input().split())
        s1, s2, cl = s1-1, s2-1, cl-1
        
        working_stations.add(s1)
        working_stations.add(s2)
        
        # Processar primeira estação
        if cl not in station_lines[s1]:
            station_lines[s1].add(cl)
            lines_per_station[cl] += 1
            for existing_line in station_lines[s1]:
                if existing_line != cl:
                    main_graph[cl].add(existing_line)
                    main_graph[existing_line].add(cl)
        
        # Processar segunda estação
        if cl not in station_lines[s2]:
            station_lines[s2].add(cl)
            lines_per_station[cl] += 1
            for existing_line in station_lines[s2]:
                if existing_line != cl:
                    main_graph[cl].add(existing_line)
                    main_graph[existing_line].add(cl)
    
    if max(lines_per_station) == n:
        return 0, None
    if len(working_stations) < n:
        return -1, None
    return 1, main_graph

def bfs(start, graph):
    global farthest_neighbor
    total_nodes = l - 1
    visited = set()
    queue = deque([(start-1, 0)])
    visited.add(start-1)
    max_distance = 0
    
    while queue:
        node, dist = queue.popleft()
        for neighbor in graph[node]:
            if neighbor not in visited:
                visited.add(neighbor)
                new_dist = dist + 1
                max_distance = max(max_distance, new_dist)
                queue.append((neighbor, new_dist))
                farthest_neighbor = neighbor + 1
                total_nodes -= 1
                
    return -1 if total_nodes > 0 else max_distance

def calculate_distance(input_value, graph):
    if input_value <= 0:
        print(input_value)
        return
        
    if bfs(1, graph) == -1:
        print(-1)
        return
        
    print(bfs(farthest_neighbor, graph))
    return

def main():
    input_value, graph = read_input()
    calculate_distance(input_value, graph)
    return

if __name__ == "__main__":
    main()