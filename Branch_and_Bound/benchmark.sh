meson compile -C build

for strategy in DFS BFS MLP; do
    echo "Running with strategy: $strategy"
    k=1

    for instance in data/instances/*; do
        echo $instance >> ./output.txt
        
        echo "Processando $instance com $strategy"
        echo "Instance $k of 9"
        
        ./build/src/tsp ${instance} $strategy >> ./output.txt
        
        k=$(($k+1))
    done
    
    echo "-" >> ./output.txt
done
