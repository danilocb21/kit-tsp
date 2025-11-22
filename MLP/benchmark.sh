meson compile -C build

k=1

for instance in data/instances/*; do
	echo $instance >> ./output.txt
	
	echo "Processando $instance"
	echo "Instance $k of 22"
	
	./build/src/mlp ${instance} >> ./output.txt
	
	k=$(($k+1))
done

echo "-" >> ./output.txt
