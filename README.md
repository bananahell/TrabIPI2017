# Trab1IPI

Trabalho 1  

Trabalho de Introdução a Processamento de Imagens  
14/0065032 - Pedro Nogueira  
UnB  

## Specs

### docs

  O pdf do professor tá nessa pasta.  
  Tem um arquivo MyDoxygen que coloquei lá que já personalizei. Pretendo colocar a zoeira toda do doxygen lá. O .pdf final gerado pelo jeito sempre se chama refman.pdf.  

### makefile

  Só bota ```make help``` na console e escolhe uma opção, mas não se preocupem em entender esse makefile, sério...  

### lib, include e src

  Organização que o makefile entende, totalmente recomendo.  

   * lib - Recebe bibliotecas importadas (se bem que acho que isso tá meio quebrado)  
   * include - Onde vão os headers (nosso caso, .hpp's)  
   * src - Onde vai o código (.cpp's) e os objetos (.o's, na pasta obj)

### Esse README.md

  Deixa ele atualizado com coisa assim mais importante, seria massa.  

## Estilo

Processamentoo de Imagens. Escolhi opencv em vez de matlab. Que Deus perdoe minha alma.  
Professor passou [isso aqui](http://www.ieee.org/conferences_events/conferences/publishing/templates.html).  
Não querendo ser o chatão mas já sendo, acho importante ter um padrão de escrita (e inclusive praticar esse tipo de coisa).  
Mesmo que isso venha do Jan, esse é o método de escrita usado pela [Google](https://google.github.io/styleguide/cppguide.html).  
Outro método que valorizo é o de documentação por comentário, e eu vou estar usando [Javadocs](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html#cppblock).  
O gcov tá [aqui](http://bobah.net/d4d/tools/code-coverage-with-gcov), mas eu sinceramente to sem saco pra isso.  
Pior ainda o [gtest](https://github.com/google/googletest).  
Vou estar gerando doxygen e usando cppcheck, mas ninguém precisa mexer nisso (desde que faça o código [bonito](https://www.youtube.com/watch?v=U8HKaO8qXS4)).  
