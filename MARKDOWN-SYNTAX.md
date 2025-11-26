# Markdown Syntax Reference

## Headers
```
# H1
## H2
### H3
#### H4
##### H5
###### H6
```

## Text Formatting
```
**bold text**
*italic text*
***bold and italic***
~~strikethrough~~
`inline code`
```

## Lists

### Unordered
```
- Item 1
- Item 2
  - Nested item
  - Another nested
```

### Ordered
```
1. First item
2. Second item
   1. Nested item
   2. Another nested
```

### Task Lists
```
- [x] Completed task
- [ ] Incomplete task
```

## Links
```
[Link text](https://example.com)
[Link with title](https://example.com "Hover title")
<https://auto-link.com>
```

## Images
```
![Alt text](image.png)
![Alt text](image.png "Image title")
```

## Code Blocks

### Inline
```
Use `code` inline
```

### Fenced
````
```cpp
int main() {
    return 0;
}
```
````

### With syntax highlighting
````
```python
def hello():
    print("Hello")
```
````

## Blockquotes
```
> Single line quote
>
> Multiple paragraphs
> in a quote
>
> > Nested quote
```

## Horizontal Rules
```
---
***
___
```

## Tables
```
| Header 1 | Header 2 | Header 3 |
|----------|----------|----------|
| Cell 1   | Cell 2   | Cell 3   |
| Cell 4   | Cell 5   | Cell 6   |

| Left | Center | Right |
|:-----|:------:|------:|
| L    | C      | R     |
```

## Line Breaks
```
Two spaces at end of line  
creates a line break

Or use a blank line

for a paragraph break
```

## Escaping Characters
```
\* Escaped asterisk
\_ Escaped underscore
\` Escaped backtick
\# Escaped hash
```

## HTML (if supported)
```html
<div align="center">
  <strong>HTML works too</strong>
</div>

<br>
<hr>
```

## Footnotes
```
Here's a sentence with a footnote[^1].

[^1]: This is the footnote content.
```

## Definition Lists
```
Term
: Definition of the term

Another term
: Definition here
```

## Abbreviations
```
The HTML specification is maintained by the W3C.

*[HTML]: Hyper Text Markup Language
*[W3C]: World Wide Web Consortium
```

## Embedded Content

### YouTube (if supported)
```
[![Video Title](thumbnail.jpg)](https://youtube.com/watch?v=VIDEO_ID)
```

### Collapsible Sections
```html
<details>
<summary>Click to expand</summary>

Hidden content here

</details>
```

## Mathematical Expressions (if supported)
```
Inline: $E = mc^2$

Block:
$$
\frac{n!}{k!(n-k)!}
$$
```

## Emojis (GitHub-flavored)
```
:smile: :heart: :+1:
```

## Comments
```
[//]: # (This is a comment)
[comment]: <> (This is also a comment)
```

## Advanced Formatting

### Superscript/Subscript (if supported)
```
x^2^ (superscript)
H~2~O (subscript)
```

### Highlighting (if supported)
```
==highlighted text==
```

### Underline (HTML)
```html
<u>underlined text</u>
```

## Reference-Style Links
```
[Link text][reference]
[Another link][ref2]

[reference]: https://example.com
[ref2]: https://example.org "Optional title"
```

## Auto-linking
```
www.example.com
user@example.com
```