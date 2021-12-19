# ED2_Trabalho_2
## Grupo

- Gabriel Toledo Netto Gribel.
- Marcus Vinícius Rodrigues da Silva.
- Nícolas Moraes.
- Pedro do Couto Filgueiras.

## Problemas
  Tivemos grande dificuldade para gerar o 'make-file', mas conseguimos gerar um arquivo '.exe' que acreditamos que possa compensá-lo.

## Como executar
  Na pasta onde o executável está armazenado execute o seguinte comando:

```         
.\main [diretório do arquivo csv]
```

  Após isso o programa irá verificar se existe o arquivo "tiktok_app_reviews.bin" arquivo binário na mesma pasta do executável.
  Se não possuir um arquivo binário ele irá criar imprimindo na tela o número de registros gravados.

  Após isso, o programa irá apresentar o menu para interagir com o usuário.<br>

**1 - Teste de Ordenação:** <br>Irá pedir para o usuário inserir o tamanho do vetor que ele deseja ordenar;<br>
                        logo após gerar o vetor com valores aleatorios do arquivo binário ele ira perguntar qual método o usuário deseja ordernar o seu vetor;<br>
                        assim que for encerrado, ele irá gerar um arquivo chamado "saida.txt" com os dados sobre a ordenação.<br>
                        <br>
**2 - Hash:**  <br>Após executar, o programa irá gerar um vetor com 80 registros aleatórios;<br>
           O programa imprimirá no console o vetor e a tabela Hash;<br>
           O usuário poderá pesquisar o 'app_version' que deseja buscar e imprimirá na tela todos reviews que possuem o mesmo 'app_version'.<br>
           <br>
**3 - Modulo de Teste:**  <br>O programa irá gerar um vetor de tamanho igual a 100;<br>
                      Logo em seguida irá perguntar com qual método o usuário deseja ordernar esse vetor;<br>
                      Em seguida o programa gerará um arquivo chamado 'teste.txt' contendo o método utilizado, vetor ordenado e o número de versões repetidas.<br>
