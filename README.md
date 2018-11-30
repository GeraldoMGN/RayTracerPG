# Projeto da disciplina Processamento gráfico / CIn-UFPE
Projeto desenvolvido durante a disciplina de Processamento Gráfico com Silvio Melo em 2018.2

* O programa necessita de um arquivo de texto chamado "config" no mesmo diretório do binário e uma pasta "objs" com os meshes .obj.
* Deve rodar com qualquer .obj que contenha somente triângulos, a câmera deve ser ajustada pra cada mesh. Está implementado interpolação de normais para dar um shading mais "liso", mas só funcionará caso o .obj tenha os vetores normais e não estejam paralelos (nesse caso se teria uma normal de face).
* Para acelerar a renderização é usada threads, isso pode deixar o computador muito lento durante a renderização.
