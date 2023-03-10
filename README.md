# Trabalho 2 - Fundamento de Sistemas Embarcados

## Enunciado

<https://gitlab.com/fse_fga/trabalhos-2022_2/trabalho-2-2022-2>

## Como Executar

### Pré-Requisitos

- Clonar este projeto dentro de uma Raspberry PI;
- Possuir a lib `bme280` e `wiringPi` instaladas na Raspberry PI;
- Entre no diretório: `trabalho2`

### Compilação

- Utilize o `make` para compilar o projeto executando o seguinte comando dentro do diretório `trabalho2` do projeto:

```bash
# Clone o projeto
git clone https://github.com/DouglasMonteles/trabalho2-fse.git

# Entre no diretório
cd trabalho2-fse/trabalho2

# Elimina os binários de compilações anteriores, gera novos binários e executa a aplicação
make build

# OU

# Apaga os binários antigos
make clean

# Gera novos binários
make

# Executa a aplicação
./bin/main
```

## Gráficos dos resultados

### Temperatura de referência fixa 

![Temperatura de referência fixa](assets/grafico%20temp%20referecia%20fixa%20(temperatura).png)

![Temperatura de referência fixa - potência](assets/grafico%20temp%20referencia%20fica%20(potencia).png)

### Seguindo uma curva pré-definida (curva_reflow.csv)

![](assets/grafico%20curva%20csv%20(temperatura).png)

![](assets/grafico%20curva%20csv%20(potencia).png)

## Apresentação

Link: <https://youtu.be/t01jqrnWzoI>
