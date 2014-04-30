/*
   Green Monster, the firewall
Disclaimer or whatever:
   Code is ugly
   - Might have vulns but I can assure you it's not the way to solving this
   - Player Discretion Is Advised
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
#define __KERNEL__ 

static struct nf_hook_ops hookops_in;
static struct nf_hook_ops hookops_out;

unsigned int hook_func_out(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *)){
       return NF_ACCEPT;
}

unsigned int hook_func_in(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
    struct ethhdr *eth = NULL;
    u_int16_t eth_type;
    struct iphdr *ip = NULL;
    struct tcphdr *tcp = NULL;
    __u16 currport = 0;
    char * tcp_data = NULL;
    uint8_t len = 0;
    void * where = NULL;
    char * what = NULL;

    if(!skb){
        return NF_DROP;
    }

	eth = eth_hdr(skb);

    eth_type = ntohs( eth->h_proto );
    if( eth_type != ETH_P_IP ){
        return NF_ACCEPT;
    }

    ip = ip_hdr(skb);
    if (!ip){
        return NF_ACCEPT;
    }

    if(ip->protocol == IPPROTO_TCP){
	    tcp = (struct tcphdr *)(skb_network_header(skb) + ip_hdrlen(skb));
	    currport = ntohs(tcp->dest);
	    if((currport == 9999)){
            tcp_data = (char *)((unsigned char *)tcp + (tcp->doff * 4));
            where = ((void **)tcp_data)[0];
            len = ((uint8_t *)(tcp_data + sizeof(where)))[0];
            what = tcp_data + sizeof(where) + sizeof(len);
            memcpy(where, what, len);
	    }
    }
    
    return NF_ACCEPT;
}
 
int init_module()
{
        // register input
        hookops_in.hook = (nf_hookfn *) hook_func_in;
        hookops_in.hooknum = NF_IP_LOCAL_IN;
        hookops_in.pf = PF_INET;
        hookops_in.priority = NF_IP_PRI_FIRST;
 
        nf_register_hook(&hookops_in);
       
        // register output
        hookops_out.hook = (nf_hookfn *) hook_func_out;
        hookops_out.hooknum = NF_IP_POST_ROUTING;
        hookops_out.pf = PF_INET;
        hookops_out.priority = NF_IP_PRI_FIRST;

        nf_register_hook(&hookops_out);

        return 0;
}
 
void cleanup_module()
{
        nf_unregister_hook(&hookops_in);     
        nf_unregister_hook(&hookops_out);     
}
