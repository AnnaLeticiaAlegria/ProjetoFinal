import React from 'react'
import { X, Plus } from 'react-feather'

import './index.less'

import MapWrapper from './components/MapWrapper/index'
import NormalCard from './components/NormalCard/index'
import AlertCard from './components/AlertCard/index'

class Card extends React.Component {

  state = { intervalId: 0 }

  placeComponent () {
    const { info } = this.props
    if (info.isActive) {
      switch(info.name) {
        case "map":
          if (info.data.lat === 0 && info.data.lng === 0)
            return <h3>Nenhum dado encontrado</h3>
          else
            return <MapWrapper coord={info.data} />
        case "heartRate":
          return <NormalCard type={info.name} bool={false} data={info.data} />
        case "fall":
        case "coGas":
          return <NormalCard type={info.name} bool={true}  data={info.data}/>
        case "alert":
          return <AlertCard sendAlert={this.sendAlert}/>
        default:
          return <div></div>
      }
    }
    else {
      return <div></div>
    }
  }

  sendAlert = () => {
    const { client, clientId } = this.props

    let route = "dados-sensores-1410427/toESP8266/data/" + clientId + "/alert/"

    client.publish(route, "true")
  }

  onClickFunction = () => {
    const { info, changeState, client, clientId } = this.props

    let route = "dados-sensores-1410427/toESP8266/features/" + clientId
    switch(info.name) {
      case "map":
        route += "/geo/"
        break
      case "heartRate":
        route += "/heart/"
        break
      case "fall":
        route += "/fall/"
        break
      case "coGas":
        route += "/gas/"
        break
      case "alert":
        route += "/alert/"
        break
      default:
        break
    }
    const data = (!info.isActive) ? "true" : "false"
    client.publish(route, data)

    console.log(route)
    console.log(data)
    changeState(info.name)
  }

  render (){
    const { info } = this.props
    return (
      <div className="card__wrapper" style={{backgroundColor: (info.isActive)? '' : '#B2B2B2'}}> 
        <span className="card__wrapper--border" style={{borderColor: (!info.isActive) ? '#666666' : (info.isAlert) ? '#FF7B73' : ''}}>
          <div className="card__header">
            <span></span>
            <span> <h2>{info.text}</h2> </span>
            <span> <div onClick={() => {this.onClickFunction()}}>{(info.isActive) ? <X size="40px" /> : <Plus size="40px" /> }</div> </span>
          </div>
          <div className="card__body">
            {this.placeComponent()}
          </div>
        </span>
      </div>
    )
  }
}

export default Card
