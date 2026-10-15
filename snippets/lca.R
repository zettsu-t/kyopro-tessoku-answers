library(extrafont)
library(igraph)
library(tidyverse)

dataframe_to_tree <- function(df) {
  gr <- igraph::graph_from_data_frame(d=df, directed=FALSE)
  node_ids <- unique(sort((c(df$from, df$to))))
  mat <- igraph::distances(gr, node_ids, node_ids)
  dist <- mat[order(as.numeric(rownames(mat))), order(as.numeric(colnames(mat)))]
  list(df = df, graph=gr, dist = dist)
}

plot_tree <- function(df, gr) {
  plot(gr, layout = layout_as_tree(gr, root=1),
       vertex.size = 25, vertex.label.cex = 1.5,
       vertex.color="azure", vertex.label.color="navy", vertex.label.family="Migu 1M",
       edge.label.cex = 1.5,
       edge.color="gray70", edge.label.color="royalblue", edge.label.family="Migu 1M",
       edge.label = df$weight)
}

df_graph1 <- tibble(from=c(1,2,2,5,5,1,3,3), to=c(2,4,5,8,9,3,6,7), weight=c(300,30,40,5,6,400,50,60))
g1 <- dataframe_to_tree(df_graph1)
print(g1$gr)
print(g1$dist)
plot_tree(df = df_graph1, gr = g1$gr)

png(filename="tree1.png")
plot_tree(df = df_graph1, gr = g1$gr)
dev.off()

df_graph2 <- tibble(from=c(3,2,2,3), to=c(2,1,4,5), weight=c(100,30,40,200))
g2 <- dataframe_to_tree(df_graph2)
print(g2$gr)
print(g2$dist)
plot_tree(df = df_graph2, gr = g2$gr)

png(filename="tree2.png")
plot_tree(df = df_graph2, gr = g2$gr)
dev.off()
