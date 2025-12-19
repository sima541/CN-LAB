from mininet.topo import Topo

class LeafSpine(Topo):
    def build(self, spine_count=2, leaf_count=2, hosts_per_leaf=2):

        # create spine switches
        spines = []
        for i in range(spine_count):
            s = self.addSwitch(f's{i+1}')
            spines.append(s)

        # create leaf switches and connect hosts
        host_id = 1
        leaves = []
        for j in range(leaf_count):
            l = self.addSwitch(f'l{j+1}')
            leaves.append(l)

            # add hosts to this leaf
            for h in range(hosts_per_leaf):
                host = self.addHost(f'h{host_id}')
                host_id += 1
                self.addLink(host, l)

        # connect every leaf to every spine
        for l in leaves:
            for s in spines:
                self.addLink(l, s)

# register topology
topos = {
    'leafspine': (lambda: LeafSpine())
}
