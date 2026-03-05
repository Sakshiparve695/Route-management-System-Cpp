import mysql.connector

def connect_db():
    return mysql.connector.connect(
        host="localhost",
        user="root",
        password="Sakshi@123",
        database="smart_delivery_system"
    )

# --------------- ROAD PROGRAM -----------------

def load_graph():
    conn = connect_db()
    cursor = conn.cursor()

    cursor.execute("SELECT source_location, destination_location, distance FROM roads")

    graph = {}

    for src, dest, dist in cursor.fetchall():
        graph.setdefault(src, []).append((dest, dist))
        graph.setdefault(dest, []).append((src, dist))

    return graph

#------------------- ROUTE OPTIMISATION -----------------------

import heapq

def dijkstra(graph, start, end):
    pq = [(0, start)]
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    parent = {}

    while pq:
        current_distance, node = heapq.heappop(pq)

        if node == end:
            break

        for neighbor, weight in graph[node]:
            distance = current_distance + weight

            if distance < distances[neighbor]:
                distances[neighbor] = distance
                parent[neighbor] = node
                heapq.heappush(pq, (distance, neighbor))

    path = []
    current = end

    while current in parent:
        path.append(current)
        current = parent[current]

    path.append(start)
    path.reverse()

    return path, distances[end]


if __name__ == "__main__":
    graph = load_graph()

    start = 1
    end = 10

    path, distance = dijkstra(graph, start, end)

    print("Optimal Route:", path)
    print("Total Distance:", distance)

# -------- GRAPH VISUALIZATION ---------

import matplotlib.pyplot as plt
import networkx as nx

G = nx.Graph()

for node in graph:
    for neighbor, weight in graph[node]:
        G.add_edge(node, neighbor, weight=weight)

pos = nx.spring_layout(G)

# Draw all nodes and edges
nx.draw(G, pos, with_labels=True)

# Draw edge weights
edge_labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

# --- Highlight optimal route ---
path_edges = list(zip(path, path[1:]))
nx.draw_networkx_edges(G, pos, edgelist=path_edges, width=3)

plt.title("Smart Delivery Route Network (Optimal Route Highlighted)")
plt.show()
