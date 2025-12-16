# Research Task

## Research for issue: $ARGUMENTS

**GOAL**
Create a todo list, based on the existing code-base to achieve the goal of the provided issue for the developer.

**IMPORTANT**
Everytime you start a new step - inform the user which step you are starting

## Research Process

### Step 1 - Understand the Project
- Read `README.md`
- Execute:
```bash
tree src/
```
to identify the current file structure.

### Step 2 - Read the code
- Read the related files and the surrounding context
- Read classes and code blocks relevant to the issue at hand

### Step 3 - Brainstorm possible solutions
Brainstorm a possible solution based on all gathered knowledge.

- Document:
  - Concise solution summary 
    - A short, high-level description of the proposed solution. 
    
  - A consice High-level todo list, written as if assigned by a teacher
    - An outline of the main steps required to implement the solution
    - Each item should describe what to do conceptually
      - Avoid detailed instructions
      - Avoid substeps
    - The items should only describe approaches and structures 
    - The focus is on general steps, patterns, and structures
    - Each step should be concise
    - The todo list should only contain rough high-level instructions to be used for academic purposes 
    
  - Study and research topics for the user (for academic purposes)
    - A list of subjects the user can investigate further, such as:
      - Names of suggested design or code patterns 
      - Names of recommended technologies, frameworks, and features 
      - Links or titles of relevant documentation and reference materials 
      - Any other related concepts or resources (e.g., architectural styles, best practices, etc.)

### Step 4 - Write the output file
- Write the plan as a well-structured markdown following the template provided at the bottom of this file
- Choose a fitting name based on the goal of the issue (1-3 words in snake_case)
- Save the file under `.claude/plans/`

Template:
```markdown
# Todo list: [Name that you chose for the issue]

## Summary: [The brief description you came up with]

## Proposed Solution:
[Description of solution]

## Todo list:
Steps:
1) [ ] [Step 1]
2) [ ] [Step 2]
3) [ ] [Step 3]
4) [ ] [etc]

## Docs and research material:
Relevant topics and documentation:
- Name of topic without a external link
- [Name of topic/documentation with a external link](https://link_to_documentation_or_blog_etc.com)
- etc
```