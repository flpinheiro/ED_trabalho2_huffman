# ED_trabalho2_huffman

Código do projeto proposto pelo professor Bruno Macchiavello, como forma de avaliação da disciplina Estrutura de Dados (116319) turma D do semestre 2-2018 do curso de licenciatura em Computação da Universidade de Brasília, realizado pelo estudante Felipe Luís Pinheiro em C.

O objetivo desse trabalho era desenvolver dois programas, sendo o primeiro chamado comprime que tem por função receber um arquivo de texto em codificação ascii simples e convertelo em um arquivo binário mais um arquivo de codificação binário e o segundo sendo o descomprime que tem por objetivo receber os arquivos de codificação e binário gerados pelo anterior e devolver o arquivo de texto ascii inicial. 

Este projeto foi completamente desenvolvido em C, para compilar e executar siga os passos abaixo em ambiente linux, no terminal.

gcc comprime.c -o comprime.o

./comprime entrada.txt

sendo entrada.txt seu arquivo de texto que se deseja comprimir e posteriormente execute

gcc descomprime.c -o descomprime.o

./descomprime.o codificacao.txt huffman.bin

sendo codificacao.txt o arquivo de codificação e huffman.bin o arquivo binário gerado pelo programa comprime.
