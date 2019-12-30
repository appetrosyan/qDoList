# A To-do List manager Written in QML

This is a Linux and Unix spcific to-do list manager that aims to provide a convenient user interface. 

It's very close in spirit to [todoist](https://todoist.com/?lang=en), but with a few major philosophical differences. 

The goal of this project was not to create a functional application, but rather a way to train in front-end development, whilst not cluttering the already over-abundant plethora of websites. 

## Design Philosophy

**qDoList** aspires to become the *emacs* of task management. Emacs has a lot of task scheduling features, especially in org mode, which is my personal daily driver (qDoList doesn't do mobile sync yet). 

### Locality

Data is local. There's no centralised server, nor should there be. If you want sync, use Dropbox, or any other cloud service. 

Files can be edited outside the program, and ideally, they could also be encrypted. 

### Command-driven interface

Like *Vim* and *Emacs*, qDoList aspires to be a fully command-driven program. As such you can do everything you want with a command. Right now the auto-suggestion feature is criminally malformed, and I promise to get back to fixing it when I have a chance. That being said, pretty much anything you can do with the help of the UI, you can do with both just keyboard shortcuts, and more importantly as a command. 

What I would like to add at some point is a unix slot, allowing to communicate with an instance from a helper program, so that completing tasks can be automated. 

Of course, this is not limited to actions, but you can also filter things with full JavaScript. Unfortunately I couldn't find a better option to do it yet. 

### Natural Language-driven

Ideally, you want to be able to ask qDoList to "Call dad monday" and "Pull request: check, test, sign off, merge" which would be parsed as a scheduled task and a task with four sub-tasks. 

Thankfully, you live in an ideal world and those things actually do work! Just try it, it's magical. 

### Minimalist

I don't want the program to become bloated. I want for it to be extensible, but it should be confined to "don't pay for what you don't use". In other words, if I want a feature to be supported I add an API and an extension, rather than extend the base program. 

Currently, since the program saves the data as Raw JSON, the extensions can simply add fields which will be carried over. Not ideal if you don't want to keep your 200+ to-do list in memory, but people rarely exceed that. 

## Future developments

### An extension API

Ideally, an easy way to save/parse other to-do application data. Integrating with Habitica would be useful too. 

### Extra natural language processing

Work in progress. 

### Internationalisation

Currently natural language processing works via a non-localised English only regexp-based parser. If you didn't cringe reading that sentence, you are not a software engineer... 

### Auto-suggesions API

Currently not all supported commands show up in auto-completion. Moreover, it's not automatic, so the problem may arise if I change the syntax a little bit. 

### Refactoring

I want for the JSON backend to be just that. I want for the program to be able to easily change the default file type and for that file type to be as stable as possible. 

### Make use of introspection

QObject allows for you to have a dynamic representation of all the properties. I want to not have to specify what a todo object is, rather, to specify field types (i.e. a DATE, a TIME INTERVAL, a BOOLEAN) and to have all of that logic to be offloaded to a configuration file. 

That would be a long trip to implement in QML, which is probably why this project would go the Telegram route. 

## Donate

No reason for you to donate, except you think that people like me should be encouraged. If you do think that you want to help, right now, I need a developer far more than I need money. 

