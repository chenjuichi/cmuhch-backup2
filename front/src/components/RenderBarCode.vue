<template>
<v-app>
  <MyBarcode :barcode_data="regFields" @pressPrint="onPressPrint"></MyBarcode>
</v-app>
</template>

<script>
import MyBarcode from '../components/BarCode.vue';

export default {
  name: 'RenderBarCode',

  components: {
    MyBarcode,
  },

  //props: [
  //  'sidebar', 'drawer'
  //],
  props: {
    sidebar: {
      type: Array,
      required: true,
    },
    drawer: {
      type: Boolean,
      required: true,
    },
  },

  data () {
    return {
      fields: [],
      show_barcode: false,
      regFields: [],
      waiting_in_total_tags: 0,
    };
  },

  watch: {
    sidebar(val) {
      this.fields.push(val);
      console.log("sidebar:", val);
      console.log("fields:", this.fields);
    },

    drawer(val) {
      if (val) {
        this.show_barcode=val;
        console.log("drawer ok!");
        this.formField();
      }
      console.log("drawer:", val);
    },

    //codeInfo(val) {
    //  this.fields.push(val);
    //  console.log("codeInfo:", val);
    //},
  },

  created () {
    console.log("RenderBarCode, created()...");
    //this.fields = [];
    //this.fields = this.sidebar;
    //console.log('props: ', this.sidebar[0]);
  },

  mounted() {
    console.log("RenderBarCode, mounted()...");
  },

  computed: {

  },

  methods: {
    ////handleScroll(event) {
      // Any code to be executed when the window is scrolled
      //this.isUserScrolling = (window.scrollY > 0);
    ////  console.log('calling handleScroll');
    ////},

    formField() {
      //let test_obj = { stockInTag_cnt: undefined };

      this.regFields=[];
      let temp_len=this.fields.length;
      console.log("temp_len: ", temp_len);
      for (let i=0; i < temp_len; i++) {
        //console.log("temp_len, type: ", typeof(this.fields[temp_len-1][i]), this.fields[temp_len-1][i]);
        if (typeof(this.fields[temp_len-1][i]) !="undefined") {

          console.log("RenderBarCode, bar obj: ", 'stockInTag_cnt' in this.fields[temp_len-1][i]);
          if ('stockInTag_cnt' in this.fields[temp_len-1][i]) {   //入庫
            let temp_tags=this.fields[temp_len-1][i].stockInTag_cnt;
            for (let j=0; j < temp_tags; j++) {
              this.fields[temp_len-1][i]['stockInTag_cnt']=1;
              this.regFields.push(this.fields[temp_len-1][i]);
            }
          } else {   //出庫
            let temp_tags=this.fields[temp_len-1][i].stockOutTag_cnt;
            for (let j=0; j < temp_tags; j++) {
              this.fields[temp_len-1][i]['stockOutTag_cnt']=1;
              this.regFields.push(this.fields[temp_len-1][i]);
            }
          }
        }
      }
      this.waiting_in_total_tags=this.regFields.length;
      //this.$emit('waitTags', this.waiting_in_total_tags);
    },

    onPressPrint(value) {
      this.is_print_OK=value;
      if (this.is_print_OK)
        this.$emit('waitTags', this.waiting_in_total_tags);
    },

    /*
    addFormElement(type) {
      this.isShow = true;
      if (this.isShow) {
        this.fields.push({
          'type': type,
          id: this.count++
        });
        this.isPrint = true;
      }
    },
    */
  },
}
</script>

<style scoped>

</style>