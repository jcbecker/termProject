%%=============================================================================
%% mdtuffs.cls - Classe para formatação de MDT da UFFS
%%
%% This program is free software; you can redistribute it and/or modify
%% it under the terms of the GNU General Public License as published by
%% the Free Software Foundation; either version 2 of the License, or
%% (at your option) any later version.
%%
%% This program is distributed in the hope that it will be useful,
%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%% GNU General Public License for more details.
%%
%% You should have received a copy of the GNU General Public License
%% along with this program; if not, write to the Free Software
%% Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
%%
%% Esta classe é uma adaptação de mdtuffs.cls, desenvolvido pela UFFS.
%%
%% A formatação de documentos segue as recomendações contidas na
%% Estrutura e Apresentação de Monografias, Dissertações e Teses - MDT, 
%% , disponível no site da UFFS: 
%% http://www.uffs.edu.br/index.php?site=biblio&option=com_content&view=article&id=3882&Itemid=853
%% 
%%=============================================================================

%%=============================================================================
%% Identificação
%%=============================================================================
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{mdtuffs}[2014/15/08 v1.0 Formatacao de monografias, dissertacoes e teses da UFFS]

%%=============================================================================
%% Declaração de opções
%%=============================================================================
\DeclareOption{tese}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{\tesespecificinfo}
    \def\doctypename{Tese de Doutorado}
    \def\doctypearticle{a}
	\def\doctypenameCIP{Tese (Doutorado)}
  }
}
\DeclareOption{espec}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{
      Monografia apresentada ao \@altcourse{} da \uffs{} 
      (UFFS, SC), como requisito parcial para a 
      obten{\c{c}}{\~a}o do grau de\\[6pt]
      \textbf{\@degree}
    }
    \def\doctypename{Monografia de Especializa{\c{c}}{\~a}o}
    \def\doctypearticle{a}
    \def\doctypenameCIP{Monografia (Especializa{\c{c}}{\~a}o)}	
  }
}
\DeclareOption{tg}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{
      Trabalho de conclus{\~a}o de curso de gradua{\c{c}}{\~a}o apresentado
      como requisito para obten{\c{c}}{\~a}o do grau de \@degree ~da \uffs{}.
    }
    \def\doctypename{Trabalho de Gradua{\c{c}}{\~a}o}
    \def\doctypenameIngles{Undergraduate Final Work}
    \def\doctypearticle{o}
	  \def\doctypenameCIP{Monografia (Gradua{\c{c}}{\~a}o)}
  }

}
\DeclareOption{diss}{
  \AtEndOfClass{
    \newcommand{\titlepagespecificinfo}{
        Disserta{\c{c}}{\~a}o apresentada ao Curso de Mestrado %em Computa{\c{c}}{\~a}o
        Programa de \\ P\'{o}s-Gradua{\c{c}}{\~a}o em Ci{\^e}ncia da Computa{\c{c}}{\~a} (PPGCC), \'{A}rea de Concentra{\c{c}}{\~a}o em Ci{\^e}ncia da Computa{\c{c}}{\~a}o,  
%        do \@altcourse{}, \'{A}rea de Concentra{\c{c}}{\~a}o em Computa{\c{c}}{\~a}o, 
        da \uffs{} (UFFS, SC), \\ como requisito parcial para obten{\c{c}}{\~a}o do grau de\\[6pt]
      \textbf{\@degree}
    }
    \def\doctypename{Disserta{\c{c}}{\~a}o de Mestrado}
    \def\doctypearticle{a}
    \def\doctypenameIngles{Master's Dissertation}
	  \def\doctypenameCIP{Disserta{\c{c}}{\~a}o (Mestrado)}
  }
}
\DeclareOption{english}{\@englishtrue\OptionNotUsed}\newif\if@english\@englishfalse
% TODO: incluir opcao twoside
\DeclareOption{oneside}{\AtEndOfClass{\@twosidefalse}\OptionNotUsed}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}
\ProcessOptions\relax


%% Dissertação apresentada ao Curso de Mestrado do Programa de
%% Pós-Graduação em Ciência da Computação, Área de Concentração em
%% Ciência da Computação, da Universidade Federal da Fronteira Sul (UFFS, SC),
%% como requisito parcial para obtenção do grau de
%% Mestre em Ciência da Computação.

%% Monografia apresentada ao Curso de Ciência da Computação
%% da Universidade Federal da Fronteira Sul (UFFS, SC),
%% como requisito parcial para obtenção do grau de
%% Bacharel em Ciência da Computação.


%% Trabalho de Graduação apresentado ao Curso de Ciência da Computação -- Bacharelado da Universidade Federal da Fronteira Sul (UFFS, SC), como requisito parcial para obtenção do grau de Bacharel em Ciência da Computação.


%%=============================================================================
%% Comandos e definições gerais
%%=============================================================================
\def\uffs{Universidade Federal da Fronteira Sul}
\def\uffsIngles{Federal University of Fronteira Sul} 

\newcommand{\course}[1]{\gdef\@course{#1}}
\newcommand{\altcourse}[1]{\gdef\@altcourse{#1}}
\newcommand{\institute}[1]{\gdef\@institute{#1}}
\newcommand{\degree}[1]{\gdef\@degree{#1}}

\newcommand{\trabalhoNumero}[1] {\gdef\@trabalhoNumero{#1}}

\newcommand{\location}[3]{
        \gdef\@city{#1}
        \gdef\@state{#2}
        \gdef\@country{#3}
}
\location{Chapec\'{o}}{SC}{Brasil}

\newcommand\LargeA{\@setfontsize\LargeA{18pt}{18.1pt}}
\newcommand\LargeB{\@setfontsize\LargeB{16pt}{16.1pt}}
\newcommand\LargeC{\@setfontsize\LargeC{14pt}{14.1pt}}

%Para saber se é uma ``autora'' em vez de ``autor''
\newif\ifautora
%Para saber se é ``orientadora'' em vez de ``orientador''
\newif\iforientadora
\newif\ifcoorientadora

\newif\ifchap\chaptrue
\newif\ifappendix
\newif\ifannex

\newif\if@hyperLoaded\@hyperLoadedfalse
\AtBeginDocument{\@ifpackageloaded{hyperref}{\@hyperLoadedtrue}{}}

\newcommand{\nocontentsline}[3]{}
\newcommand{\tocless}[2]{\bgroup\let\addcontentsline=\nocontentsline#1{#2}\egroup}

%%=============================================================================
%% Herança da classe 'report' e inclusão de pacotes e arquivos necessários
%%=============================================================================
\LoadClass[12pt,a4paper,oneside]{report}
\RequirePackage[english,brazilian]{babel}
\input{brazilian.babel} % definições para textos em português
\input{english.babel}   % definições para textos em inglês
\if@english
        \selectlanguage{english}
        \AtBeginDocument{\selectlanguage{english}}
\fi
\RequirePackage{lastpage}
\RequirePackage{indentfirst} 
\RequirePackage{natbib}
\RequirePackage{eso-pic}%para importar arquivos .pdf (catalogação \includeCIP{})
\RequirePackage{color, xcolor}
\RequirePackage{fix-cm}%para funcionar corretamente o tamanho das fontes da capa
\RequirePackage{setspace}

%%=============================================================================
%% Margens do texto
%%=============================================================================
\newlength{\mdt@topmargin}\setlength{\mdt@topmargin}{30mm}
\newlength{\mdt@pagenummargin}\setlength{\mdt@pagenummargin}{20mm}
\newlength{\mdt@bottommargin}\setlength{\mdt@bottommargin}{20mm}
\newlength{\mdt@sidemargin}\setlength{\mdt@sidemargin}{30mm}
\setlength{\marginparwidth}{25mm}


% ajuste das medidas verticais
\setlength{\topmargin}{\mdt@pagenummargin}
        \addtolength{\topmargin}{-1em} % isto é +- a altura do núm. da página
        \addtolength{\topmargin}{-1in}
\setlength{\headheight}{1em}
\setlength{\headsep}{\mdt@topmargin}
        \addtolength{\headsep}{-\mdt@pagenummargin}
\setlength{\textheight}{\paperheight}
        \addtolength{\textheight}{-\mdt@topmargin}
        \addtolength{\textheight}{-\mdt@bottommargin}
\setlength{\footskip}{\mdt@bottommargin}

% ajuste das medidas horizontais
\setlength{\oddsidemargin}{\mdt@sidemargin}
        \addtolength{\oddsidemargin}{-1in}
\setlength{\evensidemargin}{\oddsidemargin}
\setlength{\textwidth}{\paperwidth}
        \addtolength{\textwidth}{-2\mdt@sidemargin}
\setlength{\marginparsep}{\mdt@sidemargin}
        \addtolength{\marginparsep}{-\marginparwidth}
        \setlength{\marginparsep}{.5\marginparsep}

%indentar em 1,25 o primeiro parágrafo
\setlength{\parindent}{1.25cm}%

%%=============================================================================
%% Numeração das páginas
%%=============================================================================
\def\ps@mdtuffs{
        \let\@oddfoot\@empty
        \let\@evenfoot\@empty
        \def\@evenhead{{\small\thepage}}
        \def\@oddhead{\hspace*{\fill}{\small\thepage}}
}
\pagestyle{empty}
\let\ps@plain\ps@empty

%%=============================================================================
%% Definição de autor(es), orientador, co-orientador e banca examinadora
%%=============================================================================
\newcounter{ii@authorcount}
\renewcommand{\author}[2]{
        \addtocounter{ii@authorcount}{1}
        \expandafter\gdef\csname @author\alph{ii@authorcount}last\endcsname{#1}
        \expandafter\gdef\csname @author\alph{ii@authorcount}first\endcsname{#2}
        \expandafter\gdef\csname @author\alph{ii@authorcount}\endcsname{#2 #1}
}
\DeclareRobustCommand{\advisor}[4][]{
        \gdef\@advisorlast{#3}
        \gdef\@advisorfirst{#4}
        \gdef\@advisor{#1~#2~#4 #3}
		\gdef\@advisorcommittee{#2 #4 #3}
		\gdef\@advisorwithouttitle{#4 #3}
}
\DeclareRobustCommand{\coadvisor}[4][]{
        \gdef\@coadvisorlast{#3}
        \gdef\@coadvisorfirst{#4}
        \gdef\@coadvisor{#1~#2~#4 #3}
		\gdef\@coadvisorcommittee{#2 #4 #3}
		\gdef\@coadvisorwithouttitle{#4 #3}
}
\newcounter{ii@committeecount}
\DeclareRobustCommand{\committee}[4][]{
  \addtocounter{ii@committeecount}{1}
  \expandafter\gdef\csname @committee\alph{ii@committeecount}last\endcsname{#2}
  \expandafter\gdef\csname @committee\alph{ii@committeecount}first\endcsname{#3}
  \expandafter\gdef\csname @committee\alph{ii@committeecount}\endcsname{#1 #3 #2 - #4}
}



%%=============================================================================
%% Definição das palavras-chave (máx. 10)
%%=============================================================================
\newcounter{kwcounter}
\newcommand{\keyword}[1]{
        \addtocounter{kwcounter}{1}
        \expandafter\gdef\csname keyw\alph{kwcounter}\endcsname{#1}
}
\newcommand{\@cipkw}{%
        \@ifundefined{keywa}{}{1.~\expandafter\MakeUppercase\keywa\@. }%
        \@ifundefined{keywb}{}{2.~\expandafter\MakeUppercase\keywb\@. }%
        \@ifundefined{keywc}{}{3.~\expandafter\MakeUppercase\keywc\@. }%
        \@ifundefined{keywd}{}{4.~\expandafter\MakeUppercase\keywd\@. }%
        \@ifundefined{keywe}{}{5.~\expandafter\MakeUppercase\keywe\@. }%
        \@ifundefined{keywf}{}{6.~\expandafter\MakeUppercase\keywf\@. }%
        \@ifundefined{keywg}{}{7.~\expandafter\MakeUppercase\keywg\@. }%
        \@ifundefined{keywh}{}{8.~\expandafter\MakeUppercase\keywh\@. }%
        \@ifundefined{keywi}{}{9.~\expandafter\MakeUppercase\keywi\@. }%
        \@ifundefined{keywj}{}{10.~\expandafter\MakeUppercase\keywj\@. }%
}
\newcommand{\@abstractkw}{%
        \@ifundefined{keywa}{}{\expandafter\MakeUppercase\keywa}%
        \@ifundefined{keywb}{}{. \keywb}%
        \@ifundefined{keywc}{}{. \keywc}%
        \@ifundefined{keywd}{}{. \keywd}%
        \@ifundefined{keywe}{}{. \keywe}%
        \@ifundefined{keywf}{}{. \keywf}%
        \@ifundefined{keywg}{}{. \keywg}%
        \@ifundefined{keywh}{}{. \keywh}%
        \@ifundefined{keywi}{}{. \keywi}%
        \@ifundefined{keywj}{}{. \keywj}%
}

%%=============================================================================
%% Redefinição da data (para ter dia, mês e ano separados)
%%=============================================================================
\renewcommand{\date}[3]{
  \gdef\@publday{#1}
  \gdef\@publmonth{#2}
  \gdef\@publyear{#3}
}
\date{\monthname}{\number\year}

\newcommand{\dateingles}[2]{{#1} $\text{\@publday}^{\text{#2}}$, \@publyear}

%%=============================================================================
%% Definições comuns à capa e à folha de rosto.
%% Os dados específicos do tipo de trabalho (dissertação, tese, etc.) são
%% informados definindo-se o comando \titlepagespecificinfo. Veja os
%% arquivos .def incluídos com a classe.
%%=============================================================================
\newlength{\titlepagewinwidth}\setlength{\titlepagewinwidth}{135mm}
\newlength{\titlepagewinheight}\setlength{\titlepagewinheight}{70mm}
\newlength{\titlepagewinvoffset}\setlength{\titlepagewinvoffset}{69mm} %%akkk
%\newlength{\titlepagewinvoffset}\setlength{\titlepagewinvoffset}{112mm}


\renewcommand{\maketitle}{ % capa e folha de rosto
        \begin{titlepage}
                \thetitlepage
        \end{titlepage}
        \clearpage
        \secondtitlepage
        \setcounter{page}{2}
}


%%=============================================================================
%% Capa
%%=============================================================================
\newcommand{\thetitlepage}{ 
  \noindent
  % cabeçalho
  \parbox[t]{\textwidth}{
    \centering
    \vbox to \titlepagewinvoffset{
      {\includegraphics[width=20mm, height=20mm]{figuras/uffs.eps}}\\
      {\vspace{1.5mm}}
      {\textbf{\MakeUppercase{\uffs}}}\\
      {\textbf{\MakeUppercase{Campus Chapec\'{o}}}}\\ %%campus
      {\textbf{\MakeUppercase{\@course}}}\\
    }
      
    %Autor
    \vfill
    \vspace{2cm}
	\centerline{\LargeC\textbf{\MakeUppercase{\@authora}}}

    % título
    \vfill
    \vspace{3cm}
    \parbox[b]{\titlepagewinwidth}{%
      \centering
      \vbox to \titlepagewinheight{%
      \vspace*{\fill}
      {
       \textbf{\MakeUppercase{\@title}}\par}
       \vspace{5cm}
       \vfill
      %%\centerline{\LargeC\textbf{\MakeUppercase{\doctypename}}}
      }
    }
  }
  \vspace{1cm}
  \vfill
  \centerline{\textbf{\MakeUppercase{\@city}}}%%, \@state, \@country}}
  %%\vspace{12pt}
  \centerline{\textbf{\@publyear}}
}



%%=============================================================================
%% Folha de rosto
%%=============================================================================
\newcommand{\secondtitlepage}{ 
  \noindent
  \parbox[t]{\textwidth}{
    \centering
    \begin{onehalfspacing}
        {\textbf{\MakeUppercase{\@authora}}}
    \end{onehalfspacing}
  }
  
  \vfill\vfill\vfill\vfill
  \parbox[t]{\titlepagewinwidth}{
    \centering
    \begin{onehalfspacing}
        {\textbf{\MakeUppercase{\@title}}\par}
    \end{onehalfspacing}
  }

  \vspace{3cm}
  \vbox{
    
	\flushright
    \parbox[t]{6.8cm}{
        {\fontsize{10pt}{1em}\selectfont
        	{\titlepagespecificinfo}
            \@ifundefined{@advisor}{}{
                \vfill{{\iforientadora\advisornamef\else\advisorname\fi:~\@advisor}}}
            \@ifundefined{@coadvisor}{}{
                \\[6ex]{{\ifcoorientadora\coadvisornamef\else\coadvisorname\fi:~\@coadvisor}}}
        }
	}
  }
  
  \vfill\vfill
  \noindent
  \vfill\vfill\vfill
  \begin{onehalfspacing}
    \centerline{\MakeUppercase{\@city}}%%, \@state, \@country}}
    \centerline{\@publyear}
  \end{onehalfspacing}
}



%%=============================================================================
%% Definição padrão da folha de aprovacao e CIP
%%=============================================================================
\newcommand{\makeapprove}{%
  \clearpage
  \theapprovepage
}
\newcommand{\theapprovepage}{%
  \noindent
  \vfill
  \noindent\@authorbox
  \vfill
  \noindent\approvecommitteeinfo
  \vfill
  \vfill
}

%%===================================
%%        CIP
%%===================================
\newcommand{\makeCIP}[1]{
  \clearpage
  \mbox{} \vfill
  \@cipwindow
	\vspace{2ex}
  \nominata{#1}
  %se for impresso, não deve contar a página, pois será impressa no verso da folha de aprovação
  %\setcounter{page}{2}
}

\def\@cipwindow{%
        \noindent
        \framebox[140mm]{
        \begin{minipage}{120mm}%
                \vspace*{2ex}
                \setlength{\parindent}{2em}%
                \setlength{\parskip}{1.4ex}%
                \noindent \@authoralast, \@authorafirst\par
                \@title\ / por \@authora.\ -- \@publyear.\\
                \indent\pageref{LastPage}~f.: il.; 30 cm. \par
				\indent\iforientadora\advisornamef\else\advisorname\fi: \@advisorfirst\ \@advisorlast
				\@ifundefined{@coadvisor}{}{\\ \indent\ifcoorientadora\coadvisornamef\else\coadvisorname\fi: \@coadvisorfirst\ \@coadvisorlast} \\
				\indent \doctypenameCIP \ - \uffs, \@institute, Chapec\'{o}, \@publyear. \par
                1. INTRODU\c{C}\~AO. 2. REFERENCIAL TE\'ORICO. 3. RESULTADOS. 4. IMPLEMENTA\c{C}\~AO. 5. CONCLUS\~OES E TRABALHOS FUTUROS. \@cipkw I.~\@advisorlast, \@advisorfirst. \@ifundefined{@coadvisorfirst}{II.~T\'{\i}tulo.}{II.~\@coadvisorlast, \@coadvisorfirst. III.~T\'{\i}tulo.}\par
        \vspace{4ex}
        \end{minipage}%
        }%
    }

\newcommand{\nominata}[1]{
 \vspace{3ex}
 % \noindent
 % Ficha catalográfica elaborada por \\
 % {#1} \\
 % Biblioteca Central da UFFS \\
  \hrule
  \vskip 10\p@
  \noindent \copyright \ \@publyear \\
  Todos os direitos autorais reservados a \@authora. A reprodu{\c{c}}{\~a}o de partes ou do todo deste trabalho s\'{o} poder\'{a} ser feita mediante a cita{\c{c}}{\~a}o da fonte. \\
  E-mail: {#1}
}

\newcommand{\includeCIP}[1]{
   %impresso no verso da folha de rosto, nesse caso não deve contar a página
   %\setcounter{page}{1}
   \clearpage\mbox{}
   \AddToShipoutPicture{\includegraphics{#1}}
   \clearpage
   \ClearShipoutPicture
}

\def\@authorbox{%
  \parbox[t]{\textwidth}{%
    \centering
    \MakeUppercase{\textbf{\@authora}}\\
    \@ifundefined{@authorb}{}{\MakeUppercase{\textbf{\@authorb}\\}}%
	\@ifundefined{@authorc}{}{\MakeUppercase{\textbf{\@authorc}\\}}
	\@ifundefined{@authord}{}{\MakeUppercase{\textbf{\@authord}\\}}
	   
    
    \vspace{4cm}
    \MakeUppercase{\LargeC\textbf{\@title}}\\
	  
	 \vspace{1.5cm}
		 {Trabalho de conclus{\~a}o de curso de gradua{\c{c}}{\~a}o
		 apresentado como requisito para obten{\c{c}}{\~a}o do grau de 
		 \@degree ~da~ \uffs}.
		 
	\vspace{3cm}
	\flushleft
	Orientador: Prof. ~\@advisorcommittee \\
	\vspace{0.5cm}
	Este trabalho de conclus{\~a}o de curso foi defendido e aprovado pela banca em:  \_\_\_\_/{}\_\_\_\_/{}\_\_\_\_ 
	   
  }
}



\newcommand{\approvecommitteeinfo}{%
  \parbox[t]{\textwidth}{
	\vspace{3cm}
    \leftline{\MakeUppercase{Banca Examinadora:}}
    \vspace{1.5cm}\vfill 	
    \@ifundefined{@advisorcommittee}{}{
		\parbox[t]{\textwidth}{
			\centering
			\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ \\
			\@advisorcommittee ~-~UFFS
		}
	}
    \vspace{1.5cm}\vfill
    \@ifundefined{@committeea}{}{
		\parbox[t]{\textwidth}{
			\centering
			\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ \\
			\@committeea
		}
    }
    
    \vspace{1.5cm}\vfill
    \@ifundefined{@committeeb}{}{
		\parbox[t]{\textwidth}{
			\centering
			\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ \\
			\@committeeb
		}
	}
  }
}



%%=============================================================================
%% Ajuste do sumário (sem indentação) 
%%=============================================================================
\newlength{\ii@chapnumindent}\setlength{\ii@chapnumindent}{1 em}
\renewcommand\@dotsep{1.5}
\renewcommand*{\l@chapter}[2]{
  \addpenalty{-\@highpenalty}
  \vskip -2.5ex \@plus\p@
  \setlength\@tempdima{1.5em}
  \begingroup
  \parindent \z@ \rightskip \@pnumwidth
  \parfillskip -\@pnumwidth
  \leavevmode\bfseries
  \advance\leftskip\@tempdima
  \hskip -\leftskip
  %      esta é a definicao original da classe report:
  %      #1\nobreak\hfil \nobreak\hb@xt@\@pnumwidth{\hss #2}\par
  \@dottedtocline{0}{0em}{\ii@chapnumindent}{\MakeUppercase{#1}}{#2}
  %        \@dottedtocline{0}{0em}{\ii@chapnumindent}{\MakeUppercase{#1}}{#2}
  \penalty\@highpenalty
  \endgroup
}
\renewcommand*{\l@section}[2]{\@dottedtocline{1}{0em}{1.7em}{\textbf{#1}}{\hss #2}}
\renewcommand*{\l@subsection}{\@dottedtocline{2}{0em}{2.4em}}
\renewcommand*{\l@subsubsection}{\@dottedtocline{3}{0em}{3em}}
\renewcommand*{\l@paragraph}{\@dottedtocline{4}{0em}{3.8em}}
\newif\if@aftertoc
\@aftertocfalse
\renewcommand{\tableofcontents}{%
        \chapter*{\contentsname}
        \@starttoc{toc}\@aftertoctrue%
}

%%=============================================================================
%% Adicionar todos os tipos de seção, subseção, etc no sumário
%%=============================================================================
%\part            | -1 (book and report) 0 (article)
%\chapter         | 0 (book and report only)
%\section         | 1
%\subsection      | 2
%\subsubsection   | 3
%\paragraph       | 4
%\subparagraph    | 5
\setcounter{secnumdepth}{5}
\setcounter{tocdepth}{5}

%%=============================================================================
%% Definição das listas de abreviaturas e siglas e de símbolos
%%=============================================================================
\newenvironment{listofabbrv}[1]{
        \chapter*{\listabbrvname}
        \begin{list}{\textbf{??}}{
                \settowidth{\labelwidth}{#1}
                \setlength{\labelsep}{1em}
                \setlength{\itemindent}{0mm}
                \setlength{\leftmargin}{\labelwidth}
                \addtolength{\leftmargin}{\labelsep}
                \setlength{\rightmargin}{0mm}
                \setlength{\itemsep}{.1\baselineskip}
                \renewcommand{\makelabel}[1]{\makebox[\labelwidth][l]{##1}}
        }
}{
        \end{list}
}
\newenvironment{listofsymbols}[1]{
        \chapter*{\listsymbolname}
        \begin{list}{\textbf{??}}{
                \settowidth{\labelwidth}{#1}
                \setlength{\labelsep}{1em}
                \setlength{\itemindent}{0mm}
                \setlength{\leftmargin}{\labelwidth}
                \addtolength{\leftmargin}{\labelsep}
                \setlength{\rightmargin}{0mm}
                \setlength{\itemsep}{.1\baselineskip}
                \renewcommand{\makelabel}[1]{\makebox[\labelwidth][l]{##1}}
        }
}{
        \end{list}
}

%%=============================================================================
%% Resumo (abstract) e Abstract (englishabstract)
%%=============================================================================
\renewenvironment{abstract}{%
  \chapter*{\abstractname}
  \noindent
}
{%
  \setlength{\baselineskip}{\baselineskip}
  \par\vfill
  
  \noindent
  \keywordsname: \@abstractkw.
}

\newcommand{\abstractheaderbox}{%
  \parbox[t]{\textwidth}{
  }
}

\newenvironment{englishabstract}[5]{
        \if@openright\cleardoublepage\else\clearpage\fi%
        \gdef\@englishkeywords{#3}
        \begin{otherlanguage}{english}
          \chapter*{\abstractname}
				%	\vspace{-1cm}
          \noindent\englishabstractheaderbox{#1}{#2}{#4}{#5}
          \indent
}{
          \par\vfill
          \noindent{\keywordsname:} \@englishkeywords.
       \end{otherlanguage}
}

\newcommand{\englishabstractheaderbox}[4]{%
}

%%=============================================================================
%% Formatação dos capítulos.
%% Os comandos \@chapter e \@schapter são redefinidos para que a
%% numeração de páginas seja correta (deve aparecer somente a partir do
%% primeiro capítulo do texto, mas não nas páginas iniciais de
%% capítulos).
%% Os comandos \@makechapterhead e \@makeschapterhead fazem a
%% formatação dos títulos propriamente ditos.
%%=============================================================================

        %  \settowidth{\ii@chapnumindent}{{\textbf{\@chapapp}}}
        %  \addtolength{\ii@chapnumindent}{1.8em}
\newcommand{\ii@chapterfont}{\@setfontsize\Large{14}{20}}
\def\@chapapp{}
\def\@chapter[#1]#2{%
        \refstepcounter{chapter}%
        \ifchap\addcontentsline{toc}{chapter}{\protect\numberline{\@chapapp\thechapter}#1}\fi%
        \addtocontents{lof}{\protect\addvspace{0\p@}}%original = 10\p@ 
        \addtocontents{lot}{\protect\addvspace{0\p@}}%
        \pagestyle{mdtuffs}\let\ps@plain\ps@mdtuffs%
        \@makechapterhead{#2}\@afterheading
}
\def\@makechapterhead#1{%
        \ifchap
          \reset@font\noindent{\ii@chapterfont\textbf{\@chapapp\thechapter\hspace{0.5em}\MakeUppercase{#1}}\par}
				\else
				                      %\@setfontsize\Large{12}{20} se for o padrão da seção%
				  \reset@font\noindent{\ii@chapterfont\textbf{\MakeUppercase{\@chapapp}\thechapter\hspace{0.5em}-- #1}\par}
				\fi
				\ifappendix
 		      \addcontentsline{loap}{section}{\MakeUppercase{\@chapapp}\thechapter\hspace{0.5em}-- #1}
				\fi
				\ifannex
					\addcontentsline{loan}{section}{\MakeUppercase{\@chapapp}\thechapter\hspace{0.5em}-- #1}
				\fi
        \addvspace{25pt}
}
\def\@schapter#1{%
        \ifchap\if@aftertoc\addcontentsline{toc}{chapter}{#1}\fi\fi%
        \addtocontents{lof}{\protect\addvspace{0\p@}}%Original = 10\p@
        \addtocontents{lot}{\protect\addvspace{0\p@}}%
        \@makeschapterhead{#1}\@afterheading
}
\def\@makeschapterhead#1{%
        \reset@font\centerline{\ii@chapterfont\centering\textbf{\MakeUppercase{#1}}}
        \addvspace{25pt}
}


%%=============================================================================
%% Formatação das seções
%%=============================================================================
%\renewcommand{\section}{%name, level, indent, beforeskip, afterskip,style
%        \@startsection{section}{1}{0pt}{18pt}{18pt}{\reset@font\bfseries\@setfontsize\Large{12}{20}}
%}
\renewcommand\section{\@startsection{section}{1}{0pt}%
                                   {18pt}%
                                   {18pt}%
                                   {\reset@font\bfseries\@setfontsize\Large{12}{20}}}
\renewcommand\subsection{\@startsection{subsection}{2}{0pt}%
                                   {12pt}%
                                   {18pt}%
                                   {\reset@font\@setfontsize\Large{12}{20}}}																	
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{0pt}%
                                   {12pt}%
                                   {18pt}%
                                   {\reset@font\@setfontsize\Large{12}{20}}}																																		
\renewcommand\paragraph{\@startsection{paragraph}{4}{0pt}%
                                   {12pt}%
                                   {18pt}%
                                   {\reset@font\@setfontsize\Large{12}{20}}}	
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{0pt}%
                                   {1ex}%
                                   {-1em}%
                                   {\reset@font\@setfontsize\Large{12}{20}}}	
%%=============================================================================
%% Redefinição do conteúdo das listas de figuras e tabelas
%%=============================================================================
\long\def\@makecaption#1#2{%
  \vskip\abovecaptionskip
  \sbox\@tempboxa{#1~-- #2}%
  \ifdim \wd\@tempboxa >\hsize
    #1~-- #2\par
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\@tempboxa\hfil}%
  \fi
  \vskip\belowcaptionskip}
	
\newif\if@captionlisting\@captionlistingtrue
\long\def\@caption#1[#2]#3{%
  \par
  \if@captionlisting%
    \addcontentsline{\csname ext@#1\endcsname}{#1}%
        {\protect\numberline{\csname fnum@#1\endcsname~--}{\ignorespaces #2}}%
  \fi%
  \begingroup
    \@parboxrestore
    \if@minipage
      \@setminipage
    \fi
    \normalsize
    \@makecaption{\csname fnum@#1\endcsname}{\ignorespaces #3}\par
  \endgroup%
}
\renewcommand*{\l@figure}{\@dottedtocline{1}{0em}{5.5em}}
\let\l@table\l@figure

%%=============================================================================
%% Redefinição do formato de citações longas
%%=============================================================================
\renewenvironment{quote}{
        \footnotesize
        \begin{list}{}{\setlength{\leftmargin}{40mm}\item\relax}
}{
        \end{list}
}

%%=============================================================================
%% Ajuste do formato das citações e referências
%%=============================================================================
\setlength{\bibhang}{0pt}
\let\cite\citep


%%=============================================================================
%% Formatação dos Apêndices 
%%=============================================================================

\newcommand{\appendixpage}{%
  \if@openright\cleardoublepage\else\clearpage\fi
  \thispagestyle{plain}%
  \if@twocolumn\onecolumn\@tempswatrue\else\@tempswafalse\fi
	\if@hyperLoaded
	  \phantomsection
	\fi
  \null\vfil
  \markboth{}{}%
  {\centering
   \interlinepenalty \@M
   \normalfont
   \Huge \bfseries \MakeUppercase\appendixsname \par \hspace{5.5cm}{\textcolor{gray}{\rule{10.5cm}{2.5mm}}}}%
	 \addcontentsline{toc}{chapter}{\appendixsname}	
  \vfil\newpage
  \if@twoside
    \if@openright
      \null
      \thispagestyle{empty}%
      \newpage
    \fi
  \fi
  \if@tempswa
    \twocolumn
  \fi
}


\renewcommand{\appendix}[1][\appendixname]{%
        \chapfalse
				\appendixtrue
				\annexfalse
				\appendixpage
        \setcounter{chapter}{0}%
				\setcounter{section}{0}%
        \renewcommand{\thechapter}{\Alph{chapter}}%
        \setcounter{tocdepth}{0}%
        \@captionlistingfalse%
        \gdef\@chapapp{#1\space}%
}

\newcommand\listofappendix{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listappendixname}%
      \@mkboth{%
          \MakeUppercase\listappendixname}%
         {\MakeUppercase\listappendixname}%
    \@starttoc{loap}%
    \if@restonecol\twocolumn\fi
}

%%=============================================================================
%% Formatação dos Anexos
%%=============================================================================
\newcommand{\annexpage}{%
  \if@openright\cleardoublepage\else\clearpage\fi
  \thispagestyle{plain}%
  \if@twocolumn\onecolumn\@tempswatrue\else\@tempswafalse\fi
	\if@hyperLoaded
	  \phantomsection
	\fi	
  \null\vfil
  \markboth{}{}%
  {\centering
   \interlinepenalty \@M
   \normalfont
   \Huge \bfseries \MakeUppercase\annexsname \par \hspace{6.2cm}{\textcolor{gray}{\rule{9.8cm}{2.5mm}}}}%
	 \addcontentsline{toc}{chapter}{\annexsname}
  \vfil\newpage
  \if@twoside
    \if@openright
      \null
      \thispagestyle{empty}%
      \newpage
    \fi
  \fi
  \if@tempswa
    \twocolumn
  \fi
}	

\newcommand{\annex}[1][\annexname]{%
        \chapfalse
				\appendixfalse
				\annextrue
				\annexpage
        \setcounter{chapter}{0}%
				\setcounter{section}{0}%
        \renewcommand{\thechapter}{\Alph{chapter}}%
        \setcounter{tocdepth}{0}%
        \@captionlistingfalse%
        \gdef\@chapapp{#1\space}%
}

\newcommand\listofannex{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listannexname}%
      \@mkboth{%
          \MakeUppercase\listannexname}%
         {\MakeUppercase\listannexname}%
    \@starttoc{loan}%
    \if@restonecol\twocolumn\fi
}	

%%=============================================================================
%% Notas de rodapé
%%=============================================================================

% Numeração sequencial ao longo do texto
\def\@removefromreset#1#2{{%
  \expandafter\let\csname c@#1\endcsname\@removefromreset
  \def\@elt##1{%
    \expandafter\ifx\csname c@##1\endcsname\@removefromreset
    \else
      \noexpand\@elt{##1}%
    \fi}%
  \expandafter\xdef\csname cl@#2\endcsname{%
    \csname cl@#2\endcsname}}}
	
\@removefromreset{footnote}{chapter}	

% Filete de 3cm a partir da margem esquerda
\renewcommand\footnoterule{%
  \kern-3\p@
  \hrule\@width.19\columnwidth
  \kern2.6\p@}

%Sem indentação o texto da nota de rodapé  
\renewcommand\@makefntext[1]{%
  \noindent\makebox[0.5em][r]{\@makefnmark}~~#1}
