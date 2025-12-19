<!-- Assignment 14 : Custom Leaf-Spine Topology in Mininet -->

## Assignment 14

### Objective
To understand the **Mininet Python API** by creating a **custom Leaf–Spine topology** that can be scaled by increasing the switch radix.

---

### Exercise
Create a **custom Leaf-Spine topology** in Mininet using **Python**, where:
- Leaf switches connect to hosts.
- Spine switches interconnect all leaf switches.
- The topology can be easily scaled by changing parameters such as number of leaf switches, spine switches, and hosts per leaf.
- A **fat-tree topology** may be used as a reference.

---

### Steps / Procedure

#### 1. Writing Custom Topology Code
- Use Mininet’s Python API.
- Create a custom topology class by extending the `Topo` class.
- Add:
  - Spine switches
  - Leaf switches
  - Hosts
- Connect:
  - Each leaf switch to all spine switches.
  - Hosts to leaf switches.

---

#### 2. Running the Custom Topology
- Save the Python file (for example: `leaf_spine.py`).
- Run the topology using:
```bash
sudo python3 leaf_spine.py
3. Verifying the Topology
Use Mininet CLI commands:

bash
Copy code
net
pingall
Ensure connectivity between all hosts.

Example Leaf–Spine Structure
less
Copy code
        Spine1     Spine2
          |  \     /  |
          |   \   /   |
        Leaf1   Leaf2   Leaf3
         | |     | |     | |
        h1 h2   h3 h4   h5 h6
Spine switches provide high-bandwidth interconnection.

Leaf switches connect directly to hosts.

Architecture is scalable and commonly used in data centers.

Files Included
File Name	Description
leaf_spine.py	Python script implementing custom Leaf-Spine topology
README.md	Assignment documentation

Learning Outcomes
Understanding Mininet Python API.

Creating custom and scalable network topologies.

Learning Leaf-Spine architecture used in data centers.

Understanding topology scalability using switch radix.

Gaining practical experience with virtual network experiments.

